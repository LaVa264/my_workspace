#pragma once
#include <unistd.h>
#include <list>
#include <memory>
class process;
class thread_scheduler_interface
{
public:
    virtual void context_switch() const = 0;
};

class preemptive_thread_scheduler : public thread_scheduler_interface
{
    class preemptive_thread_scheduler_impl;
    std::unique_ptr<preemptive_thread_scheduler_impl> impl;

public:
    void context_switch() const override;
};

class cooperative_thread_scheduler : public thread_scheduler_interface
{
    class cooperative_thread_scheduler_impl;
    std::unique_ptr<cooperative_thread_scheduler_impl> impl;

public:
    void context_switch() const override;
};