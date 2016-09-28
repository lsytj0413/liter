#include "sequenceTest.h"

#include <gtest/gtest.h>
#include <liter/sequence.hpp>
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

    using f1 = typename liter::make_sequence<2, 1>::type;
    auto v1 = std::is_same<f1, liter::sequence<1>>::value;

    EXPECT_EQ(v1, true);

    using f2 = typename liter::make_sequence<4, 2, 2>::type;
    auto v2 = std::is_same<f2, liter::sequence<2>>::value;

    EXPECT_EQ(v2, true);
}


TEST_F(SequenceTest, testOneNumDesc)
{
    using f = typename liter::make_sequence<0, 1, -1>::type;
    auto v = std::is_same<f, liter::sequence<1>>::value;

    EXPECT_EQ(v, true);

    using f1 = typename liter::make_sequence<1, 2, -1>::type;
    auto v1 = std::is_same<f1, liter::sequence<2>>::value;

    EXPECT_EQ(v1, true);

    using f2 = typename liter::make_sequence<2, 4, -2>::type;
    auto v2 = std::is_same<f2, liter::sequence<4>>::value;

    EXPECT_EQ(v2, true);
}


TEST_F(SequenceTest, testReverseZeroNum)
{
    using f = typename liter::reverse_sequence<liter::sequence<>>::type;
    auto v = std::is_same<f, liter::sequence<>>::value;

    EXPECT_EQ(v, true);
}


TEST_F(SequenceTest, testReverseOneNum)
{
    using f = typename liter::reverse_sequence<liter::sequence<1>>::type;
    auto v = std::is_same<f, liter::sequence<1>>::value;

    EXPECT_EQ(v, true);

    using f1 = typename liter::reverse_sequence<liter::sequence<0>>::type;
    auto v1 = std::is_same<f1, liter::sequence<0>>::value;

    EXPECT_EQ(v1, true);

    using f2 = typename liter::reverse_sequence<liter::sequence<73>>::type;
    auto v2 = std::is_same<f2, liter::sequence<73>>::value;

    EXPECT_EQ(v2, true);
}


TEST_F(SequenceTest, testReverseMultiNum)
{
    using f = typename liter::reverse_sequence<liter::sequence<0, 2, 3>>::type;
    auto v = std::is_same<f, liter::sequence<3, 2, 0>>::value;

    EXPECT_EQ(v, true);

    using f1 = typename liter::reverse_sequence<liter::sequence<0, 2>>::type;
    auto v1 = std::is_same<f1, liter::sequence<2, 0>>::value;

    EXPECT_EQ(v1, true);

    using f2 = typename liter::reverse_sequence<liter::sequence<2, 3, 57, 43>>::type;
    auto v2 = std::is_same<f2, liter::sequence<43, 57, 3, 2>>::value;

    EXPECT_EQ(v2, true);
}


TEST_F(SequenceTest, testMultiNumIncr)
{
    using f = typename liter::make_sequence<5>::type;
    auto v = std::is_same<f, liter::sequence<0, 1, 2, 3, 4>>::value;

    EXPECT_EQ(v, true);

    using f1 = typename liter::make_sequence<5, 2, 1>::type;
    auto v1 = std::is_same<f1, liter::sequence<2, 3, 4>>::value;

    EXPECT_EQ(v1, true);

    using f2 = typename liter::make_sequence<10, 4, 2>::type;
    auto v2 = std::is_same<f2, liter::sequence<4, 6, 8>>::value;

    EXPECT_EQ(v2, true);
}


TEST_F(SequenceTest, testMultiNumDesc)
{
    using f = typename liter::make_sequence<0, 5, -1>::type;
    auto v = std::is_same<f, liter::sequence<5, 4, 3, 2, 1>>::value;

    EXPECT_EQ(v, true);

    using f1 = typename liter::make_sequence<1, 5, -2>::type;
    auto v1 = std::is_same<f1, liter::sequence<5, 3>>::value;

    EXPECT_EQ(v1, true);
}
