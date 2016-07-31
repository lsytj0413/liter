#include "binaryTest.h"

#include <gtest/gtest.h>
#include <liter/utils/binary.h>

TEST_F(BinaryTest, testBinary8)
{
    EXPECT_EQ(8, liter::binary_to<1000>());
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
