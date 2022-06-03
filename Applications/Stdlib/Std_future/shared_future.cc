#include <future>
#include <chrono>
#include <thread>
#include <vector>
#include "logger.hh"

void task_handler(const std::shared_future<void> &future)
{
    if (future.valid())
    {
        future.get();
        _LOG << "Un block this thread!! \n";
    }
}
int main()
{
    std::promise<void> promise;
    std::shared_future<void> shared_future = promise.get_future();

    std::vector<std::thread> workers;
    const unsigned int size = std::thread::hardware_concurrency();
    workers.reserve(size);
    for (int i = 0; i < size; i++)
    {
        workers.push_back(std::thread{task_handler, shared_future});
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));
    _LOG << size <<"\n";
    promise.set_value();

    for (std::thread &it : workers)
    {
        if (it.joinable())
        {
            it.join();
        }
    }
}