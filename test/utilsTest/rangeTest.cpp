#include "rangeTest.h"

#include <gtest/gtest.h>
#include <liter/range.hpp>
#include <vector>
using std::vector;


TEST_F(RangeTest, testIntSum1)
{
    auto sum = 0;
    for(auto&& i : liter::range(10))
    {
        sum += i;
    }

    EXPECT_EQ(sum, 45);
}

TEST_F(RangeTest, testIntSum2)
{
    auto sum = 0;
    for(auto&& i : liter::range(2, 10))
    {
        sum += i;
    }

    EXPECT_EQ(sum, 44);
}

TEST_F(RangeTest, testIntSum3)
{
    auto sum = 0;
    for(auto&& i : liter::range(1, 10, 2))
    {
        sum += i;
    }

    EXPECT_EQ(sum, 25);
}

TEST_F(RangeTest, testFloatSum1)
{
    float sum = 0;
    for(auto&& i : liter::range(10.0f))
    {
        sum += i;
    }

    EXPECT_FLOAT_EQ(sum, 45.0f);
}

TEST_F(RangeTest, testFloatSum2)
{
    float sum = 0;
    for(auto&& i : liter::range(2.0f, 10.0f))
    {
        sum += i;
    }

    EXPECT_FLOAT_EQ(sum, 44.0f);
}

TEST_F(RangeTest, testFloatSum3)
{
    float sum = 0;
    for(auto&& i : liter::range(1.0f, 10.0f, 3))
    {
        sum += i;
    }

    EXPECT_FLOAT_EQ(sum, 12.0f);
}

TEST_F(RangeTest, testDoubleSum1)
{
    double sum = 0;
    for(auto&& i : liter::range(10.0))
    {
        sum += i;
    }

    EXPECT_FLOAT_EQ(sum, 45.0);
}

TEST_F(RangeTest, testDoubleSum2)
{
    double sum = 0;
    for(auto&& i : liter::range(2.0, 10.0))
    {
        sum += i;
    }

    EXPECT_FLOAT_EQ(sum, 44.0);
}

TEST_F(RangeTest, testDoubleSum3)
{
    double sum = 0;
    for(auto&& i : liter::range(1.0, 11.1, 2.5))
    {
        sum += i;
    }

    EXPECT_FLOAT_EQ(sum, 30.0);
}

TEST_F(RangeTest, testCharNum)
{
    int sum = 0;
    for(auto&& i : liter::range('0', '9'))
    {
        sum += i;
    }

    int sum2 = 0;
    char c = '0';
    while(c < '9')
    {
        sum2 += c;
        c++;
    }

    EXPECT_EQ(sum, sum2);
}

TEST_F(RangeTest, testCharAlpha)
{
    int sum = 0;
    for(auto&& i : liter::range('a', 'z', 2))
    {
        sum += i;
    }

    int sum2 = 0;
    char c = 'a';
    while(c < 'z')
    {
        sum2 += c;
        c++;
        c++;
    }

    EXPECT_EQ(sum, sum2);
}

TEST_F(RangeTest, testVectorSum)
{
    vector<int> t = {1, 3, 5, 7, 9};
    int sum = 0;
    for(auto&& i : liter::range(t.size()))
    {
        sum += t[i];
    }

    EXPECT_EQ(sum, 1+3+5+7+9);
}
