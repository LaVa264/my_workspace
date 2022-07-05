#include "http_client.hh"
#include "http_client_manager.hh"


http_client::http_client(connected_socket&& fd, const ipv4_addr& host, const bool& keep_alive)
    : m_fd {std::move(fd)}
    , m_read_buf {m_fd.input()}
    , m_write_buf {m_fd.output()}
    , m_host_address {host}
    , m_keep_alive {keep_alive}
    , m_closed {false}
    , m_limit {1}
    , m_shard_number {engine().cpu_id()}
    {
        
    }

future<std::unique_ptr<reply>> http_client::forward_request(std::unique_ptr<request> req, std::unique_ptr<reply> rep)
{
    if(m_closed)
    {
        print("[%u] Server connection is closed, please make new connection. \n", m_shard_number);
        return make_exception_future<std::unique_ptr<reply>>(std::exception());
    }

    print("[%u] Forwarding the request to the server ... \n",m_shard_number);
    std::unordered_map<sstring, sstring> old_rep_header = rep->_headers;

    req = convert_request_header(std::move(req));

    sstring req_str = convert_request_to_sstring(request(*(req.get())));
            
    return m_limit.wait().then([req_str, this, old_rep_header, rep = std::move(rep)]() mutable -> future<std::unique_ptr<reply>>
    {  
        return m_write_buf.write(req_str).then([this]() mutable -> future<>
        { 
            return m_write_buf.flush(); 
        }).then([this, old_rep_header, rep = std::move(rep)]() mutable -> future<std::unique_ptr<reply>>
        {
            m_parser.init();
            return m_read_buf.consume(m_parser).then([this, old_rep_header, rep = std::move(rep)]() mutable -> future<std::unique_ptr<reply>>
            {
                print("[%u] Reading response message from the server ... \n", m_shard_number);
                // 1. Read HTTP response header.
                if (m_parser.eof()) 
                {
                    rep->set_status(httpd::reply::status_type::internal_server_error, "Fail to parse backend response.");
                    rep->done("txt");
                    return make_ready_future<std::unique_ptr<reply>>(std::move(rep));
                }
                    
                std::unique_ptr<http_response> res = m_parser.get_parsed_response();

                rep->set_version(res->_version);
                rep->_headers = std::move(res->_headers);

                rep = convert_reply_header(std::move(rep), old_rep_header);
                        
                auto con_len_it = rep->_headers.find("Content-Length");
                if (con_len_it == rep->_headers.end()) 
                {
                    print("[%u] HTTP response does not contain: Content-Length. The message does not contain data.\n", m_shard_number);
                    rep->done("html");
                    return make_ready_future<std::unique_ptr<reply>>(std::move(rep));
                }

                size_t content_len = std::stoi(con_len_it->second);
                        
                // 2. Read HTTP response body.
                return m_read_buf.read_exactly(content_len).then([content_len, this, rep = std::move(rep)] (temporary_buffer<char> buf) mutable ->  future<std::unique_ptr<reply>>
                {
                    if(content_len != buf.size() || !buf.get())
                    {
                        print("[%u] Incomplete content, response failure to client. \n", m_shard_number);
                        rep->set_status(httpd::reply::status_type::internal_server_error, "Fail to read content from Backend.");
                        rep->done("txt");
                        return make_ready_future<std::unique_ptr<reply>>(std::move(rep));
                    }
                    rep->_content = sstring(buf.get());
                    rep->done();
                    return make_ready_future<std::unique_ptr<reply>>(std::move(rep));
                });
            }); 
        });
    }).finally([this]()
    {
        m_limit.signal();
    });
}

future<> http_client::close()
{
    if(m_closed)
    {
        return make_ready_future<>();
    }

    print("[%u] Closing the connection ...\n", m_shard_number);
    return seastar::when_all_succeed(m_read_buf.close(), m_write_buf.close()).then([this]() -> future<>
    {
        m_closed = true;
        return make_ready_future<>();
    }).handle_exception([this](std::exception_ptr ep)
    {
        print("[%u] Ignore close exceptions.\n", m_shard_number);
        return make_ready_future<>();
    });
}

ipv4_addr http_client::get_host_address() const
{
    return m_host_address;
}

bool http_client::is_closed() const
{
    return m_closed;
}

std::unique_ptr<request> http_client::convert_request_header(std::unique_ptr<request> req)
{   
    sstring conn = req->get_header("Connection");
    if(m_keep_alive)
    {
        req->_headers["Connection"] = "Keep-Alive";
        req->_headers["Keep-Alive"] = "timeout=10, max=100";
    } else
    {
        req->_headers["Connection"] = "Close";
    }

    sstring host = req->get_header("Host");
    if(host != "")
    {
        host = inet_ntoa(in_addr{net::ntoh(m_host_address.ip)});
        host += ":";
        host += to_sstring(m_host_address.port);
        req->_headers["Host"] =host;
    }
    return std::move(req);
}
    
std::unique_ptr<reply> http_client::convert_reply_header(std::unique_ptr<reply> rep, const std::unordered_map<sstring, sstring>& old_rep_header)
{   
    auto con_type_it = rep->_headers.find("Content-Type");
    if (con_type_it == rep->_headers.end()) 
    {
        rep->set_mime_type("text/html");
    }

    auto host_backend_it = rep->_headers.find("Host");
    auto host_proxy_server_it = old_rep_header.find("Host");
    if (host_backend_it != rep->_headers.end() && host_proxy_server_it != old_rep_header.end()) 
    {
        rep->_headers["Host"] = old_rep_header.find("Host")->second;
    } else
    {
        rep->_headers.erase("Host");
    }

    auto conn_backend_it = rep->_headers.find("Connection");
    auto conn_proxy_server_it = old_rep_header.find("Connection");
    if (conn_backend_it != rep->_headers.end() && conn_proxy_server_it != old_rep_header.end()) 
    {
        rep->_headers["Connection"] = old_rep_header.find("Connection")->second;
    } else
    {
        rep->_headers.erase("Connection");
        rep->_headers.erase("Keep-Alive");
    }

    return std::move(rep);
}

sstring http_client::convert_request_to_sstring(const request& req)
{
    std::string result = std::string(req._method);
    result += " ";
    result += req._url;

    if (req.query_parameters.size() > 0)
    {
        result += "?";
        for(auto it : req.query_parameters)
        {
            result += it.first;
            result += "=";
            result += it.second;
            result += "&";
        }
        result.pop_back();
    }

    result += " HTTP/";
    result += req._version;
    result += "\n";

    for(auto it : req._headers)
    {
        result += it.first;
        result += ": ";
        result += it.second;
        result += "\n";
    }
    result += "\n";
    result += req.content;
        
    return result;
}

http_client::~http_client()
{
    if(!m_closed)
    {
        close();
    }
}