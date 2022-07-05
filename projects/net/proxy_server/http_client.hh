#pragma once
#include "http/http_response_parser.hh"
#include "net/dns.hh"
#include "core/future.hh"
#include "net/api.hh"
#include "net/net.hh"
#include "net/inet_address.hh"
#include "core/semaphore.hh"
#include "core/future-util.hh"
#include "http/httpd.hh"
#include "http/handlers.hh"
#include "http/function_handlers.hh"
#include "http/file_handler.hh"
#include "core/sstring.hh"
#include "core/distributed.hh"
#include "core/shared_ptr.hh"
#include "http/exception.hh"

class http_client
{
    class http_client_manager;
    connected_socket m_fd;
    input_stream<char> m_read_buf;
    output_stream<char> m_write_buf;
    http_response_parser m_parser;
    ipv4_addr m_host_address;
    bool m_keep_alive{false};
    bool m_closed{false};
    semaphore m_limit{1};
    unsigned m_shard_number{0};

public:
    explicit http_client(connected_socket &&fd, const ipv4_addr &host, const bool &keep_alive = false);

    ~http_client();

    future<std::unique_ptr<reply>> forward_request(std::unique_ptr<request> req, std::unique_ptr<reply> rep);

    future<> close();

    ipv4_addr get_host_address() const;

    bool is_closed() const;

private:
    std::unique_ptr<request> convert_request_header(std::unique_ptr<request> req);

    std::unique_ptr<reply> convert_reply_header(std::unique_ptr<reply> rep, const std::unordered_map<sstring, sstring>& old_req_header);

    static sstring convert_request_to_sstring(const request &req);
};