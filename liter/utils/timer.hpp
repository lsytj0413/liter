#pragma once

#include <chrono>

using std::chrono::time_point;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;


namespace liter
{

class timer
{
private:
    time_point<high_resolution_clock> m_begin;

public:
    timer() : m_begin(high_resolution_clock::now())
    {};

    void reset() {
        m_begin = high_resolution_clock::now();
    };

    template <typename Duration=std::chrono::milliseconds>
    int64_t elapsed() const {
        return duration_cast<Duration>(high_resolution_clock::now() - m_begin).count();
    };

    int64_t elapsed_micro() const {
        return elapsed<std::chrono::microseconds>();
    };

    int64_t elapsed_nano() const {
        return elapsed<std::chrono::nanoseconds>();
    };

    int64_t elapsed_seconds() const {
        return elapsed<std::chrono::seconds>();
    };

    int64_t elapsed_minutes() const {
        return elapsed<std::chrono::minutes>();
    };

    int64_t elapsed_hours() const {
        return elapsed<std::chrono::hours>();
    };
};

}

