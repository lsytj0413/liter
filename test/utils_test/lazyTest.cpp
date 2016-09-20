#include "lazyTest.h"

#include <gtest/gtest.h>
#include <liter/lazy.h>
#include <iostream>


int func0(){
    return 0;
};

int func1(int i){
    std::cout << i << std::endl;
    return i;
};

int func2(int i, int j){
    return i + j;
};


TEST_F(LazyTest, testArgs0)
{
    auto l = liter::lazy(func0);
    EXPECT_EQ(false, l.init_p());

    auto v = l.value();
    EXPECT_EQ(v, 0);

    EXPECT_EQ(true, l.init_p());
}

TEST_F(LazyTest, testArgs1)
{
    int i = 100;
    auto l1 = liter::lazy(func1, i);
    EXPECT_EQ(false, l1.init_p());

    auto v = l1.value();
    EXPECT_EQ(v, 100);

    EXPECT_EQ(true, l1.init_p());
}

TEST_F(LazyTest, testArgs2)
{
    int i = 200;
    int j = 4;
    auto l2 = liter::lazy(func2, i, j);
    EXPECT_EQ(false, l2.init_p());

    auto v = l2.value();
    EXPECT_EQ(v, 204);

    EXPECT_EQ(true, l2.init_p());
}
