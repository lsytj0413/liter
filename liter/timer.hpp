// @file timer.h
// @brief 计时器
// @author
// @version
// @date

#pragma once

#include <chrono>

using std::chrono::time_point;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;


namespace liter
{

// @class timer
// @brief 计时器类
class timer
{
private:
    time_point<high_resolution_clock> m_begin;

public:
    timer() : m_begin(high_resolution_clock::now())
    {};

    // @function
    // @brief 重置计时器
    // @return
    void reset() {
        m_begin = high_resolution_clock::now();
    };

    // @function
    // @brief 返回间隔, 默认毫秒
    // @return int64_t
    template <typename Duration=std::chrono::milliseconds>
    int64_t elapsed() const {
        return duration_cast<Duration>(high_resolution_clock::now() - m_begin).count();
    };

    // @function
    // @brief 返回微秒间隔
    // @return int64_t
    int64_t elapsed_micro() const {
        return elapsed<std::chrono::microseconds>();
    };

    // @function
    // @brief 返回纳秒间隔
    // @return int64_t
    int64_t elapsed_nano() const {
        return elapsed<std::chrono::nanoseconds>();
    };

    // @function
    // @brief 返回秒间隔
    // @return int64_t
    int64_t elapsed_seconds() const {
        return elapsed<std::chrono::seconds>();
    };

    // @function
    // @brief 返回分钟间隔
    // @return int64_t
    int64_t elapsed_minutes() const {
        return elapsed<std::chrono::minutes>();
    };

    // @function
    // @brief 返回小时间隔
    // @return int64_t
    int64_t elapsed_hours() const {
        return elapsed<std::chrono::hours>();
    };
};

}

