#include "timerTest.h"

#include <gtest/gtest.h>

#include <liter/timer.hpp>
#include <thread>


TEST_F(TimerTest, testElapsed)
{
    liter::timer t;

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    auto t1 = t.elapsed();
    EXPECT_GE(t1, 200);

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    auto t2 = t.elapsed();
    EXPECT_GT(t2, 399);
}


TEST_F(TimerTest, testReset)
{
    liter::timer t;

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    auto t1 = t.elapsed();

    auto t2 = t.elapsed();
    EXPECT_GE(t2, t1);

    t.reset();
    auto t3 = t.elapsed();
    EXPECT_LT(t3, t1);

    EXPECT_GE(t3, 0);
    EXPECT_LT(t3, 10);
}
