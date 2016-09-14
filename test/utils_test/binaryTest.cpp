#include "binaryTest.h"

#include <gtest/gtest.h>
#include <liter/utils/binary.hpp>


TEST_F(BinaryTest, testBinary8)
{
    auto v = liter::binary<1000>::value;
    EXPECT_EQ(8, v);
}

TEST_F(BinaryTest, testBinary5)
{
    EXPECT_EQ(5, liter::binary_to<101>());
}

TEST_F(BinaryTest, testBinary17)
{
    EXPECT_EQ(17, liter::binary_to<10001>());
}

TEST_F(BinaryTest, testBinary1)
{
    EXPECT_EQ(1, liter::binary_to<1>());
}

TEST_F(BinaryTest, testShift1)
{
    auto v = liter::shift_bit<1>::value;
    EXPECT_EQ(2, v);
}

TEST_F(BinaryTest, testShift3)
{
    auto v = liter::shift_bit<3>::value;
    EXPECT_EQ(8, v);
}

TEST_F(BinaryTest, testShift7)
{
    auto v = liter::shift_bit<7>::value;
    EXPECT_EQ(128, v);
}
