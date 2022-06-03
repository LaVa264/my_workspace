#include <future>
#include <chrono>
#include "logger.hh"

int main()
{
    { // Example 1: Create new function asynchronously.
        std::string str{"Larva"};
        std::future<std::size_t> result = std::async(
            std::launch::async,
            [=](const std::string &str) -> std::size_t {              // This lambda function block two seconds and return length of string.
                std::this_thread::sleep_for(std::chrono::seconds(2)); // Sleep this thread for two seconds.
                std::size_t res = str.length();                       // Get length of strings.
                _LOG << "Length of '" << str << "' is " << res << std::endl;
                return res;
            },
            std::ref(str));

        // ... Do some thing.

        // waits for the result, returns if it is not available for the specified timeout duration (three seconds).
        if (std::future_status status = result.wait_for(std::chrono::seconds(3)); status == std::future_status::deferred)
        { // Handles case that the state of asynchronous operation is deferred.
            _LOG << "The operation is deferred, using get() or wait() method to run asynchronous operations \n";
        }
        else if (status == std::future_status::ready)
        { // Handles case that the state of asynchronous operation is ready to get data.
            if (result.valid())
            { // Checks if the future refer to a shared state.
                _LOG << "Result is available to get: " << result.get() << std::endl;
            }
        }
        else if (status == std::future_status::timeout)
        { // Handles case that the state of asynchronous operation is timeout.
            _LOG << "The operation is running, please wait a second to get data \n";
        }
    }
}