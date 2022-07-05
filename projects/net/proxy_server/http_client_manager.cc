#include "http_client_manager.hh"
#include "http_client.hh"
#include "semaphore.h"




http_client_manager::http_client_manager()
    : m_mode {connection_mode::non_persistent_connection}, m_shard_number {engine().cpu_id()}
{

}

future<std::unique_ptr<reply>> http_client_manager::forward_request(const ipv4_addr& host, const sstring &path, std::unique_ptr<request> req, std::unique_ptr<reply> rep)
{
    if(m_mode == connection_mode::non_persistent_connection)
    {
        return make_connect(host).then([this, path, req = std::move(req), rep = std::move(rep)](lw_shared_ptr<http_client> ptr) mutable -> future<std::unique_ptr<reply>>
        {
            return ptr->forward_request(std::move(req), std::move(rep)).then([this, ptr] (std::unique_ptr<reply> rep_send) mutable -> future<std::unique_ptr<reply>>
            {
                return ptr->close().then([ptr, rep_send = std::move(rep_send)]() mutable -> future<std::unique_ptr<reply>>
                {
                    ptr = nullptr;
                    return make_ready_future<std::unique_ptr<reply>>(std::move(rep_send));
                });
            }).handle_exception([this](std::exception_ptr ep) -> future<std::unique_ptr<reply>>
            {
                return make_ready_future<std::unique_ptr<reply>>(std::move(generate_fail_to_connect_reply()));
            });
        });
    } else if(m_mode == connection_mode::persistent_connection)
    {
        static thread_local semaphore limit(1);

        return limit.wait().then([this, host, path, req = std::move(req), rep = std::move(rep)]() mutable -> future<std::unique_ptr<reply>> 
        {
            print("[%u] Finding old persistent connections ... \n", m_shard_number);
            return find_persistent_connection(host).then([this, path, req = std::move(req), rep = std::move(rep)](lw_shared_ptr<http_client> ptr) mutable -> future<std::unique_ptr<reply>>
            {
                limit.signal();
                return ptr->forward_request(std::move(req), std::move(rep)).then([this, ptr] (std::unique_ptr<reply> rep_send) mutable -> future<std::unique_ptr<reply>>
                {
                    ptr = nullptr;
                    return make_ready_future<std::unique_ptr<reply>>(std::move(rep_send));
                }).handle_exception([this](std::exception_ptr ep) -> future<std::unique_ptr<reply>>
                {
                    return make_ready_future<std::unique_ptr<reply>>(std::move(generate_fail_to_connect_reply()));
                });
            }).handle_exception([this](std::exception_ptr ep) -> future<std::unique_ptr<reply>>
            {
                limit.signal();
                try 
                {
                    if (ep) 
                    {
                        std::rethrow_exception(ep);
                    }
                } catch(const std::exception& e) 
                {
                    print("[%u] Caught exception %s. \n", m_shard_number, e.what());
                }
                return make_ready_future<std::unique_ptr<reply>>(std::move(generate_fail_to_connect_reply()));
            });
        });
    } else
    {
        return make_ready_future<std::unique_ptr<reply>>(std::move(generate_fail_to_connect_reply()));
    }
}

future<> http_client_manager::stop()
{
    return close_all_persistent_connection();
}


future<> http_client_manager::set_mode(const connection_mode &mode)
{
    if(m_mode != mode)
    {
        m_mode = mode;
    }
    return make_ready_future<>();
}


std::unique_ptr<reply> http_client_manager::generate_fail_to_connect_reply() const
{
    auto rep = std::make_unique<reply>();
    rep->set_status(httpd::reply::status_type::internal_server_error, "Fail to make connect with Server.");
    rep->done("txt");
    return std::move(rep);
}

future<lw_shared_ptr<http_client>> http_client_manager::make_connect(const ipv4_addr& host, const bool& persistent_connection)
{
    return engine().net().connect(make_ipv4_address(host)).then([this, host, persistent_connection](connected_socket fd) mutable -> future<lw_shared_ptr<http_client>>
    {
        print("[%u] Established new connection with server %s:%d \n", m_shard_number, inet_ntoa(in_addr{net::ntoh(host.ip)}), host.port);
            
        lw_shared_ptr<http_client> ptr = make_lw_shared<http_client>(std::move(fd), host, persistent_connection);
        return make_ready_future<lw_shared_ptr<http_client>>(ptr);
    }).handle_exception([this, host](std::exception_ptr ep) -> future<lw_shared_ptr<http_client>>
    {
        print("[%u] Fail to connect to server %s:%d \n",m_shard_number, inet_ntoa(in_addr{net::ntoh(host.ip)}), host.port);
        return make_exception_future<lw_shared_ptr<http_client>>(ep);
    });
}

future<lw_shared_ptr<http_client>> http_client_manager::find_persistent_connection(const ipv4_addr &host)
{
    std::string address = std::string(inet_ntoa(in_addr{net::ntoh(host.ip)})) + ':' + std::to_string(host.port);
    auto it = m_persistent_connections.find(address);

    if(it != m_persistent_connections.end() && it->second && !it->second->is_closed())
    {
        print("[%u] Use a persistent connection that already exists with %s.\n",m_shard_number, address.c_str());
        return make_ready_future<lw_shared_ptr<http_client>>(it->second);
    }

    return make_connect(host, true).then([this, host, address](lw_shared_ptr<http_client> ptr) -> future<lw_shared_ptr<http_client>> 
    {
        print("[%u] Make new persistent connection with %s. \n",m_shard_number, address.c_str());
        m_persistent_connections[address] = ptr;
      //  m_persistent_connections.insert({address, ptr});
        return make_ready_future<lw_shared_ptr<http_client>>(ptr);
    }).handle_exception([this, host, address](std::exception_ptr ep) -> future<lw_shared_ptr<http_client>>
    {
        print("[%u] Fail to make connect to server %s. \n", m_shard_number, address.c_str());
        return make_exception_future<lw_shared_ptr<http_client>>(ep);
    });

}

future<> http_client_manager::close_all_persistent_connection()
{
    return parallel_for_each(m_persistent_connections, [](auto it)
    {
        if(it.second)
        {
            return it.second->close();
        }
        return make_ready_future<>();
    }).then([this]()
    {
        m_persistent_connections.clear();
        return make_ready_future<>();
    });
}
