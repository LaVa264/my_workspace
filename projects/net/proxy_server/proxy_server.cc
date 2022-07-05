#include "proxy_server.hh"
#include "http_client_manager.hh"

proxy_server::proxy_server(const sstring &name, ipv4_addr &&backend_addr)
    : m_http_server {new httpd::http_server(name)}
    , m_monitor {make_lw_shared<http_monitor>(*this)}
    , m_backend_address {std::move(backend_addr)}
    , m_client_manager {make_shared<http_client_manager>()}
    , m_shard_number {engine().cpu_id()}
{
    m_client_manager->set_mode(connection_mode::non_persistent_connection);
}

httpd::http_server* proxy_server::get_http_server()
{
    return m_http_server;
}

future<std::unique_ptr<reply>> proxy_server::handle(const sstring &path, std::unique_ptr<request> req, std::unique_ptr<reply> rep)
{
    uint64_t cur = m_http_server->current_connections();
    print("[%u] Received a new request, Handling it ... Current connection: %lu\n", m_shard_number, cur);

    connection_mode mode = connection_mode::non_persistent_connection;

    if(cur > maximum_of_current_connections)
    {
        mode = connection_mode::persistent_connection;
    }

    return m_client_manager->set_mode(mode).then([this, path, req = std::move(req), rep = std::move(rep)]() mutable -> future<std::unique_ptr<reply>> 
    {
        return m_client_manager->forward_request(m_backend_address, path, std::move(req), std::move(rep)).finally([this]()
        {
            print("[%u] Responded message to client. \n", m_shard_number);
        });
    }); 
}


void proxy_server::run_monitor()
{
    m_monitor->run();
}

void proxy_server::configure_routes()
{
    m_http_server->_routes.add(operation_type::GET, url("/"), this);
    m_http_server->_routes.add(operation_type::POST, url("/"), this);
    m_http_server->_routes.add(operation_type::PUT, url("/"), this);
}

future<> proxy_server::listen(ipv4_addr addr)
{
    return m_http_server->listen(addr);
}

future<> proxy_server::stop()
{
    m_monitor->stop();

    return seastar::when_all_succeed(m_http_server->stop(), m_client_manager->stop(), m_monitor->release_monitor()).finally([this]()
    {
        print("[%u] Stopped the service.\n", m_shard_number);
    });
}

void proxy_server::signal_handler(int sig)
{
    switch (sig)
    {
        case 0x0F:
        case 0x00 ... 0x09:
        {
            print("Was received kill signal [%d], logging info ...\n", sig);
            http_monitor::log_info().then([]() -> void
            {
                print("Then exiting application ...\n");
                engine().exit(EXIT_SUCCESS);
            }).handle_exception([](std::exception_ptr ep) -> void
            {
                print("Fail to create log file, exits application. \n");
                engine().exit(EXIT_FAILURE);
            });
            break;
        }
        case 0x20 ... 0x40:
        {
            print("Does not handle real time signal [%d]. \n", sig);
            break;
        }
        default:
        {
            print("Does not handle signal [%d]. \n", sig);
            break;
        }
    }
}