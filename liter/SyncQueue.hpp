// @file SyncQueue.hpp
// @brief 线程安全的队列容器模板
// @author
// @version
// @date

#pragma once

#include <list>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <liter/uncopyable.h>


namespace liter
{

// @class SyncQueue
// @brief 线程安全的队列容器模板
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

    // @function
    // @brief 添加对象
    // @param x: T的对象
    // @return
    void put(const T& x){
        add(x);
    };

    // @function
    // @brief 添加对象, 右值版本
    // @param x: T的对象
    // @return
    void put(T&& x){
        add(std::forward<T>(x));
    };

    // @function
    // @brief 获取一个对象, 阻塞到获取成功
    // @param x: T的对象, 入参
    // @return
    void take(T& x){
        std::unique_lock<std::mutex> locker(m_mutex);
        m_not_empty.wait(locker, [this](){
                return !m_queue.empty();
            });

        x = m_queue.front();
        m_queue.pop_front();
    };

    // @function
    // @brief 获取一个对象, 如果超过mis毫秒未获取到则返回失败
    // @param x: T的对象, 入参
    // @param mis: 超时毫秒数
    // @return bool, 是否获取成功
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

    // @function
    // @brief 获取多个对象
    // @param list: std::list<T>对象, 入参
    // @return
    void take(std::list<T>& list){
        std::unique_lock<std::mutex> locker(m_mutex);
        m_not_empty.wait(locker, [this](){
                return !m_queue.empty();
            });

        list = std::move(m_queue);
    };

    // @function
    // @brief 当前是否为空
    // @return bool
    bool empty() {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.empty();
    };

    // @function
    // @brief 当前元素个数
    // @return size_t
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


