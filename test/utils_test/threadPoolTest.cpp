#include "threadPoolTest.h"

#include <gtest/gtest.h>

#include <liter/thread_pool.hpp>


TEST_F(ThreadPoolTest, test)
{
    liter::ThreadPool pool;

    int g = 0;

    pool.add([&](){
            g = 1;
        });

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    EXPECT_EQ(g, 1);
}
