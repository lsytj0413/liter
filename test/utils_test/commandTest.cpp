#include "commandTest.h"

#include <gtest/gtest.h>

#include <liter/command.hpp>
#include <iostream>


static int g = 0;

void test0(){
    g++;
};

int test1(){
    return 100;
};

class A
{

public:
    int a = 0;

    int test(){
        return this->a;
    };

    int test(int i){
        return a + i;
    };

    int test(int i, int j){
        return a + i + j;
    };
};

TEST_F(CommandTest, testArgsZero)
{
    g = 0;

    auto f0 = liter::command<void>();
    f0.wrap(test0);

    EXPECT_EQ(g, 0);

    f0.execute();
    EXPECT_EQ(g, 1);

    auto f1 = liter::command<int>();
    f1.wrap(test1);

    auto v1 = f1.execute();
    EXPECT_EQ(v1, 100);

    A objA;
    objA.a = 0;

    auto f2 = liter::command<int>();
    f2.wrap(static_cast<int(A::*)()>(&A::test), &objA);

    EXPECT_EQ(objA.a, 0);
    auto v3 = f2.execute();
    EXPECT_EQ(v3, 0);

    objA.a = 1;
    EXPECT_EQ(objA.a, 1);
    auto v2 = f2.execute();
    EXPECT_EQ(v2, 1);
}
