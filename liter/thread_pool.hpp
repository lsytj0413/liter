#pragma once

#include <list>
#include <memory>
#include <thread>
#include <atomic>
#include <functional>
#include <cassert>

#include <liter/SyncQueue.hpp>
#include <liter/uncopyable.h>


namespace liter
{

class ThreadPool : uncopyable
{
public:
    using Task = std::function<void()>;
    using ThreadPtr = std::shared_ptr<std::thread>;

private:
    std::list<ThreadPtr> m_threads;
    SyncQueue<Task> m_tasks;
    std::atomic_bool m_running;
    std::once_flag m_once;

public:
    ThreadPool(size_t numThreads = std::thread::hardware_concurrency())
    {
        assert(numThreads > 0);
        start(numThreads);
    };

    ~ThreadPool(){
        stop_imp();
    }

    void stop(){
        std::call_once(m_once, [this](){
                stop_imp();
            });
    };

    void add(Task&& task){
        m_tasks.put(std::forward<Task>(task));
    };

    void add(const Task& task){
        m_tasks.put(task);
    };

private:
    void start(size_t numThreads){
        m_running = true;

        for (size_t i = 0; i < numThreads; ++i){
            m_threads.push_back(std::make_shared<std::thread>(&ThreadPool::run_in_thread, this));
        }
    };

    void stop_imp(){
        m_running = false;

        for (auto t : m_threads){
            t->join();
        }
        m_threads.clear();
    };

    void run_in_thread(){
        while(m_running){
            Task t;

            if (m_tasks.take(t, 100)){
                t();
            }
        }
    };
};

}
