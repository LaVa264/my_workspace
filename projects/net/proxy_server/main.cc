#include "proxy_server.hh"
#include "core/sleep.hh"
#include <iostream>
#include <csignal>
using namespace std::chrono_literals;

int main(int argc, char **argv)
{
    app_template app;
    /* 1. Add configuration infomation to the app. */
    app.add_options()
                ("proxy_port", boost::program_options::value<uint16_t>()->default_value(92), "Proxy Server port")
                ("proxy_ip", boost::program_options::value<std::string>()->default_value("127.0.0.83"), "Proxy Server ip")
                ("backend_ip", boost::program_options::value<std::string>()->default_value("127.0.0.1"), "Backend ip")
                ("backend_port", boost::program_options::value<uint16_t>()->default_value(80), "Backend port")
                ("name", boost::program_options::value<sstring>()->default_value("Larva server"), "Proxy Server name")
                ("process_id", boost::program_options::value<pid_t>()->default_value(getpid()), "Process ID");

    /* 2. Run the application in event loop. */
    return app.run_deprecated(argc, argv, [&]() -> void
        {
            for (auto sig : {SIGKILL, SIGQUIT, SIGTERM, SIGINT}) 
            {
                signal(sig, &proxy_server::signal_handler);
            }

            boost::program_options::variables_map& config = app.configuration();
            
            sstring name =  config["name"].as<sstring>();
            std::string proxy_ip = config["proxy_ip"].as<std::string>();
            uint16_t proxy_port = config["proxy_port"].as<uint16_t>();
            
            std::string backend_ip = config["backend_ip"].as<std::string>();
            uint16_t backend_port = config["backend_port"].as<uint16_t>();

            pid_t pid = config["process_id"].as<pid_t>();

            lw_shared_ptr<seastar::sharded<proxy_server>> server_distributer = make_lw_shared<seastar::sharded<proxy_server>>();
            
            server_distributer->start(name, ipv4_addr(backend_ip, backend_port)).then([server_distributer]() -> future<>
            {
                engine().at_exit([server_distributer]() -> future<>
                {
                    print("Stopping services ... \n");
                    sleep(5s).then([]() -> void
                    {
                        print("Cleanup Timeout, forced exit the proxy server.\n");
                        exit(EXIT_FAILURE);
                    });
                    return server_distributer->stop().then([server_distributer]() -> future<> 
                    {
                        server_distributer.~lw_shared_ptr<seastar::sharded<proxy_server>>();
                        print("Deleting the proxy server is completed, bye bye !!!\n");
                        return make_ready_future<>();
                    });
                });

                return server_distributer->invoke_on_all(&proxy_server::configure_routes);
            }).then([server_distributer]() -> future<>
            {
                print("Configuration for routes is done, starts monitoring ... \n");
                return server_distributer->invoke_on_all(&proxy_server::run_monitor);
            }).then([server_distributer, proxy_port, proxy_ip]() -> future<>
            {
                return server_distributer->invoke_on_all(&proxy_server::listen, ipv4_addr(proxy_ip, proxy_port));
            }).then([server_distributer, proxy_port, proxy_ip, pid]() -> void
            {
                print("Configuring is done, server [%d] is listening on %s:%d ... \n", pid, proxy_ip.c_str(), proxy_port);
            }).handle_exception([server_distributer](std::exception_ptr ep) -> void
            {
                print("Fail to configure the server. Exiting ... \n");
                try 
                {
                    if (ep) 
                    {
                        std::rethrow_exception(ep);
                    }
                } catch(const std::exception& e) 
                {
                    print("Caught exception %s. \n", e.what());
                }
                engine().exit(EXIT_FAILURE);
            }); 
        });
}
