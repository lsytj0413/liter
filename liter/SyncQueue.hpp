#pragma once

#include <list>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <liter/uncopyable.h>


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
        add(x);
    };

    void put(T&& x){
        add(std::forward<T>(x));
    };

    void take(T& x){
        std::unique_lock<std::mutex> locker(m_mutex);
        m_not_empty.wait(locker, [this](){
                return !m_queue.empty();
            });

        x = m_queue.front();
        m_queue.pop_front();
    };

    bool take(T& x, int mis){
        std::unique_lock<std::mutex> locker(m_mutex);
        auto r = m_not_empty.wait_for(locker, std::chrono::milliseconds(mis), [this](){
                return !m_queue.empty();
            });
        if (!r){
            return false;
        }

        x = m_queue.front();
        m_queue.pop_front();
        return true;
    };

    void take(std::list<T>& list){
        std::unique_lock<std::mutex> locker(m_mutex);
        m_not_empty.wait(locker, [this](){
                return !m_queue.empty();
            });

        list = std::move(m_queue);
    };

    bool empty() {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.empty();
    };

    size_t size() {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.size();
    };

private:
    template <typename F>
    void add(F&& f){
        std::unique_lock<std::mutex> locker(m_mutex);

        m_queue.push_back(std::forward<F>(f));
        m_not_empty.notify_one();
    };
};

}


