#include "taskTest.h"

#include <gtest/gtest.h>

#include <liter/task.h>


TEST_F(TaskTest, testGet)
{
    liter::task<int(int)> t([](int i)->int{
            return i;
        });

    EXPECT_EQ(0, t.get(0));
    EXPECT_EQ(5, t.get(5));
    EXPECT_EQ(70, t.get(70));

    liter::task<void(int)> t1([](int i){
        });
}


TEST_F(TaskTest, testThen)
{
    int g = 0;
    liter::task<int(int)> t([&](int i){
            g += i;
            return g;
        });

    EXPECT_EQ(6, t.get(6));

    g = 0;
    auto t1 = t.then([&](int i) -> void {
            g += i;
        });
    t1.get(5);

    EXPECT_EQ(g, 10);
}
