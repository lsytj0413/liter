#pragma once

#include <list>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <liter/utils/uncopyable.h>


namespace liter
{

template <typename T>
class SyncQueue : public uncopyable
{
private:
    std::list<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_not_empty;

public:
    SyncQueue()
    {};

    void put(const T& x){
        std::lock_guard<std::mutex> locker(m_mutex);

        m_queue.push_back(x);
        m_not_empty.notify_one();
    };

    void take(T& x){
        std::unique_lock<std::mutex> locker(m_mutex);
        m_not_empty.wait(locker, [this](){
                return !m_queue.empty();
            });

        x = m_queue.front();
        m_queue.pop_front();
    };

    bool empty() {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.empty();
    };

    size_t size() {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.size();
    };
};

}


