#include "commandTest.h"

#include <gtest/gtest.h>

#include <liter/command.hpp>


static int g = 0;

void test0(){
    g++;
};

TEST_F(CommandTest, testArgsZero)
{
    g = 0;

    auto f0 = liter::command<void>();
    f0.wrap(test0);

    EXPECT_EQ(g, 0);

    f0.execute();
    EXPECT_EQ(g, 1);
}
