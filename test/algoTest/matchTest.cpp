#include "matchTest.h"

#include <gtest/gtest.h>
#include <vector>
#include <string>
using std::string;
using std::vector;
#include <algorithm>

#include <liter/algo/match.hpp>
using namespace liter::algo;

// http://blog.csdn.net/v_july_v/article/details/7041827


TEST_F(MatchTest, testGenNextKpm1_1)
{
    KPM1 m;

    auto v = m.test_gen_next("abab");
    EXPECT_EQ(v.size(), 4);

    vector<int> p = {-1, 0, 0, 1};

    for(int i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], p[i]);
    }
}


TEST_F(MatchTest, testGenNextKpm1_2)
{
    KPM1 m;

    auto v = m.test_gen_next("abcdabd");
    EXPECT_EQ(v.size(), 7);

    vector<int> p = {-1, 0, 0, 0, 0, 1, 2};

    for(int i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], p[i]);
    }
}


TEST_F(MatchTest, testMatch)
{
    KPM m;

    auto i = m.match("abcdabd", "abd");
    EXPECT_EQ(i, 4);
}
