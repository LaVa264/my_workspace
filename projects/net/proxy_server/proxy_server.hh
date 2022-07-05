#pragma once
#include "http_client.hh"
#include "connection_monitor.hh"
#include <list>

class http_client_manager_interface;
class proxy_server : public handler_base
{
private:
    // ! why can not delete http_object even the service stop already done?
    // ! Because the http_server will be destroyed when proxy_server is destroyed, engine() emits segment fault-> Can not delete http_server object or use to smart pointer: lw_shared_ptr<httpd::http_server> m_http_server{nullptr};
    // TODO: Need to check http_server's listen(), stop(), and do_accept() methods.
    httpd::http_server* m_http_server{nullptr};
    lw_shared_ptr<http_monitor> m_monitor{nullptr};
    ipv4_addr m_backend_address;
    shared_ptr<http_client_manager_interface> m_client_manager{nullptr};
    static constexpr int maximum_of_current_connections = 2;
    unsigned m_shard_number {0};
public:
    explicit proxy_server(const sstring &name, ipv4_addr &&backend_addr);

    httpd::http_server* get_http_server();

    virtual future<std::unique_ptr<reply>> handle(const sstring &path, std::unique_ptr<request> req, std::unique_ptr<reply> rep) override;

    void run_monitor();

    void configure_routes();

    future<> listen(ipv4_addr addr);

    future<> stop();

    static void signal_handler(int sig);

    ~proxy_server() = default;
};