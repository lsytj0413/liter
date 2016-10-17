#include "matchTest.h"

#include <gtest/gtest.h>

#include <liter/algo/match.hpp>
using namespace liter::algo;


TEST_F(MatchTest, testGenNext)
{
    matcher m;

    auto v = m.test_gen_next("abcabcacab");

    EXPECT_EQ(v[0], -1);
    EXPECT_EQ(v[1], -1);
    EXPECT_EQ(v[2], -1);
    EXPECT_EQ(v[3], 0);
    EXPECT_EQ(v[4], 1);
    EXPECT_EQ(v[5], 2);
    EXPECT_EQ(v[6], 3);
    EXPECT_EQ(v[7], -1);
    EXPECT_EQ(v[8], 0);
    EXPECT_EQ(v[9], 1);
}
