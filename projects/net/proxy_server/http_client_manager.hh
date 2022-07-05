
#include "core/semaphore.hh"
#include "core/future-util.hh"
#include "http/httpd.hh"
#include "core/shared_ptr.hh"
#include <unordered_map>
#include "http_client.hh"

enum class connection_mode
{
    non_persistent_connection = 0,
    persistent_connection = 1
};

class http_client_manager_interface
{
public:
    virtual future<std::unique_ptr<reply>> forward_request(const ipv4_addr &host, const sstring &path, std::unique_ptr<request> req, std::unique_ptr<reply> rep) = 0;
    virtual future<> set_mode(const connection_mode &mode) = 0;
    virtual future<> stop() = 0;
    virtual ~http_client_manager_interface() = default;
};

class http_client;
struct ipv4_addr;
class http_client_manager : public http_client_manager_interface
{
public:
    explicit http_client_manager();
    
    virtual future<std::unique_ptr<reply>> forward_request(const ipv4_addr &host, const sstring &path, std::unique_ptr<request> req, std::unique_ptr<reply> rep) override;

    virtual future<> stop() override;

    virtual future<> set_mode(const connection_mode &mode) override;

    ~http_client_manager() = default;

private:
    future<lw_shared_ptr<http_client>> make_connect(const ipv4_addr &host, const bool &persistent_connection = false);

    future<lw_shared_ptr<http_client>> find_persistent_connection(const ipv4_addr &host);

    std::unique_ptr<reply> generate_fail_to_connect_reply() const;
    
    future<> close_all_persistent_connection();

private:
    std::unordered_map<std::string, lw_shared_ptr<http_client>> m_persistent_connections;
    connection_mode m_mode {connection_mode::non_persistent_connection};
    unsigned m_shard_number {0};
};
