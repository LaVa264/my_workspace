#pragma once 
#include <atomic>
#include <cstdint>
#include <chrono>
#include <vector>
#include "core/reactor.hh"

// Required to use raw pointer, atomic variables require completed types. So, needs to release the resources when stops monitoring.
using atomic_array = std::atomic<uint64_t*>;

class proxy_server;
struct http_monitor
{
    static atomic_array m_total_connections;
    static atomic_array m_current_connections;
    static atomic_array m_requests_served;
    static atomic_array m_respond_errors;
    static atomic_array m_read_errors;
    static atomic_array m_pers;

    void run();

    void stop();

    static future<> log_info();

    static future<> release_monitor();

    http_monitor(proxy_server& server);

    ~http_monitor() = default;

private:
    static void configure();

    static uint64_t get_total_of_array(const atomic_array& array);

private:
    proxy_server& m_proxy_server;
    timer<> m_timer;
    static unsigned m_shard_size;
    static std::atomic<bool> m_is_configured;
    static std::atomic<bool> m_is_released;
    unsigned m_shard_number {0};
};