#include "proxy_server.hh"
#include "connection_monitor.hh"
#include "core/sleep.hh"
using namespace std::chrono_literals;
 
atomic_array http_monitor::m_total_connections {};
atomic_array http_monitor::m_current_connections {};
atomic_array http_monitor::m_requests_served {};
atomic_array http_monitor::m_respond_errors {};
atomic_array http_monitor::m_read_errors {};
atomic_array http_monitor::m_pers {};
std::atomic<bool> http_monitor::m_is_configured {false};
unsigned http_monitor::m_shard_size {0};
std::atomic<bool> http_monitor::m_is_released {false};

http_monitor::http_monitor(proxy_server& server) : m_proxy_server {server}, m_shard_number {engine().cpu_id()}
{
    configure();
}

void http_monitor::run()
{
    m_timer.cancel();
    m_timer.set_callback([this]() -> void
        {
            auto http_server = this->m_proxy_server.get_http_server();
            if(!http_server)
            {
                print("[%u] Http server is nullptr. \n", m_shard_number);
                return;
            }

            if(!m_is_configured)
            {
                print("[%u] Monitoring is not configured!! \n.", m_shard_number);
                return;
            }

            if(m_is_released)
            {
                print("[%u] Monitor is released. \n", m_shard_number);
                return;
            }

            uint64_t old_reqs = m_requests_served[m_shard_number];
            m_total_connections[m_shard_number] = uint64_t(http_server->total_connections());
            m_current_connections[m_shard_number] = uint64_t(http_server->current_connections());
            m_requests_served[m_shard_number] = uint64_t(http_server->requests_served());
            m_read_errors[m_shard_number] = uint64_t(http_server->read_errors());
            m_respond_errors[m_shard_number] = uint64_t(http_server->reply_errors());
            if((m_requests_served[m_shard_number] - old_reqs) > m_pers[m_shard_number])
            {
                m_pers[m_shard_number] = m_requests_served[m_shard_number] - old_reqs;
            }
        });
    m_timer.arm_periodic(1s);
}

void http_monitor::stop()
{
    m_timer.cancel();
}

future<>  http_monitor::log_info()
{
    if(m_is_released)
    {
        print("[%u] Monitor is released. \n", engine().cpu_id());
        return make_exception_future<>(std::exception());
    }
    time_t t {::time(nullptr)};
    char tm[5];
    strftime(tm, sizeof tm, "%S", localtime(&t));
    char log_file_name[40];
    sprintf(log_file_name, "log_%s_%d.txt", tm, getpid());
    FILE* log_file = fopen(log_file_name, "w");
    if (log_file == NULL)
    {
        print("[%u] Unable to create file, make a exception. \n", engine().cpu_id());
        return make_exception_future<>(std::exception());
    }
    print("[%u] Logging to %s file. \n", engine().cpu_id(), log_file_name);
    char tm_log[50];
    strftime(tm_log, sizeof tm_log, "%T",  localtime(&t));
    fprintf(log_file, "|-Result of monitoring [%s]\n\n", tm_log);
    fprintf(log_file, "|-Total (Total connections)              : %lu\n", get_total_of_array(m_total_connections));
    fprintf(log_file, "|-Cur (Current connections)              : %lu\n", get_total_of_array(m_current_connections));
    fprintf(log_file, "|-Req (Requests served)                  : %lu\n", get_total_of_array(m_requests_served));
    fprintf(log_file, "|-Read error (Read errors)               : %lu\n", get_total_of_array(m_read_errors));
    fprintf(log_file, "|-Res error (Respond errors)             : %lu\n", get_total_of_array(m_respond_errors));
    fprintf(log_file, "|-Req/s (maximum of Request/second)      : %lu\n", get_total_of_array(m_pers));
    fprintf(log_file, "\n==============================================================================\n");
    fprintf(log_file, "|Shard ID  |Total     |Cur       |Req       |Read error|Res error |Req/s     |\n");
    
    for(unsigned i = 0; i < m_shard_size; i++)
    {
        fprintf(log_file, "|%4u      |%6lu    |%6lu    |%6lu    |%6lu    |%6lu    |%6lu    |\n", 
            i, m_total_connections[i], m_current_connections[i], m_requests_served[i], m_read_errors[i], m_respond_errors[i], m_pers[i]);
    }
    fprintf(log_file, "==============================================================================\n");
    
    fclose(log_file);
    return make_ready_future<>();
}

void http_monitor::configure()
{
    if(m_is_configured)
    {
        return;
    }
    m_is_configured = true;

    m_shard_size = smp::count;
    print("Configuring monitoring ...\n");

    m_total_connections = new uint64_t[m_shard_size];
    m_current_connections = new uint64_t[m_shard_size];
    m_requests_served = new uint64_t[m_shard_size];
    m_read_errors = new uint64_t[m_shard_size];
    m_respond_errors = new uint64_t[m_shard_size];
    m_pers = new uint64_t[m_shard_size];

    m_is_released =false;
    memset(m_total_connections, 0, m_shard_size * sizeof(m_total_connections[0]));
    memset(m_current_connections, 0, m_shard_size * sizeof(m_current_connections[0]));
    memset(m_requests_served, 0, m_shard_size * sizeof(m_requests_served[0]));
    memset(m_read_errors, 0, m_shard_size * sizeof(m_read_errors[0]));
    memset(m_respond_errors, 0, m_shard_size * sizeof(m_respond_errors[0]));
    memset(m_pers, 0, m_shard_size * sizeof(m_pers[0]));

}

future<> http_monitor::release_monitor()
{
    if(m_is_released)
    {
        return make_ready_future<>();
    }
    m_is_released = true;
    return sleep(1s).then([]() -> future<>
    { // Waiting for all timer done.
        delete[] m_total_connections;
        delete[] m_current_connections;
        delete[] m_requests_served;
        delete[] m_read_errors;
        delete[] m_respond_errors;
        delete[] m_read_errors;

        m_total_connections = nullptr;
        m_current_connections = nullptr;
        m_requests_served = nullptr;
        m_read_errors = nullptr;
        m_respond_errors = nullptr;
        m_read_errors = nullptr;
        return make_ready_future<>();
    });
}

uint64_t http_monitor::get_total_of_array(const atomic_array& array)
{
    uint64_t result {0};
    for(unsigned i = 0; i < m_shard_size; i++)
    {
        result += array[i];
    }
    return result;
}
