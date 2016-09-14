#include "sequenceTest.h"

#include <gtest/gtest.h>
#include <liter/sequence.h>
#include <type_traits>


TEST_F(SequenceTest, testZeroNumIncr)
{
    using f = typename liter::make_sequence<0>::type;
    auto v = std::is_same<f, liter::sequence<>>::value;

    EXPECT_EQ(v, true);
}


TEST_F(SequenceTest, testZeroNumDesc)
{
    using f = typename liter::make_sequence<0, 0, -1>::type;
    auto v = std::is_same<f, liter::sequence<>>::value;

    EXPECT_EQ(v, true);
}


TEST_F(SequenceTest, testOneNumIncr)
{
    using f = typename liter::make_sequence<1>::type;
    auto v = std::is_same<f, liter::sequence<0>>::value;

    EXPECT_EQ(v, true);
}


TEST_F(SequenceTest, testOneNumDesc)
{
    using f = typename liter::make_sequence<0, 1, -1>::type;
    auto v = std::is_same<f, liter::sequence<1>>::value;

    EXPECT_EQ(v, true);
}


TEST_F(SequenceTest, testReverseZeroNum)
{
    using f = typename liter::reverse_sequence<liter::sequence<>>::type;
    auto v = std::is_same<f, liter::sequence<>>::value;

    EXPECT_EQ(v, true);
}
