#include "Thread_scheduler.hh"
#include "Platform_scheduler.hh"

class Unix_platform_scheduler::Unix_platform_scheduler_impl
{
public:
    void display() const
    {
        std::cout << "Unix implement display!!";
    }
};

Unix_platform_scheduler::Unix_platform_scheduler() : impl{std::make_unique<Unix_platform_scheduler::Unix_platform_scheduler_impl>()} {}