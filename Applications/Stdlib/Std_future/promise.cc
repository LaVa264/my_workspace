#include <future>
#include <chrono>
#include "logger.hh"

void task_handler(std::promise<uint32_t> size_promise)
{ // Task handler in another threads.

    std::this_thread::sleep_for(std::chrono::seconds(2));
    size_promise.set_value(100);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    _LOG << "Task handler continues to handling !!\n";
}
int main()
{
    std::promise<uint32_t> size_promise;
    std::future<uint32_t> size_future = size_promise.get_future();
    std::thread thread{task_handler, std::move(size_promise)};
    uint32_t data = size_future.get();
    _LOG << "Get data: " << data << " successfully !!\n";
    _LOG << "Main thread continue to handling !!\n";
    thread.join();
}