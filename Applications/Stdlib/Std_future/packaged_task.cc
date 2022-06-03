#include <future>
#include <chrono>
#include "logger.hh"

std::size_t task_handler(const std::string &str)
{ // Task handler in another threads.
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return str.length();
}

int main()
{
    std::string str{"Larva"};
    std::packaged_task<std::size_t(const std::string &)> task(task_handler);
    std::future<std::size_t> result = task.get_future();

    std::thread thread{std::move(task), std::ref(str)};
    thread.detach();
    std::size_t length_of_string = result.get();
    _LOG << "Get data: " << length_of_string << " successfully, out of main() function \n";
}