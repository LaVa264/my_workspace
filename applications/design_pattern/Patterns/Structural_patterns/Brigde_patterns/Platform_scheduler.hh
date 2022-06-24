#pragma once
#include <memory>
#include <iostream>

class thread_scheduler_interface;
class platform_scheduler_interface
{
protected:
    thread_scheduler_interface &scheduler;
public:
    virtual void display() const = 0;
};

class Unix_platform_scheduler : public platform_scheduler_interface
{
    class Unix_platform_scheduler_impl;
    std::unique_ptr<Unix_platform_scheduler_impl> impl;

public:
    Unix_platform_scheduler();
    void display() const override;
};

class Window_platform_scheduler : public platform_scheduler_interface
{
    class Window_platform_scheduler_impl;
    std::unique_ptr<Window_platform_scheduler_impl> impl;

public:
    void display() const override;
};