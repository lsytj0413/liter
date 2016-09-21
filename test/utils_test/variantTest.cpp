#include "variantTest.h"

#include <gtest/gtest.h>
#include <string>

#include <liter/variant.hpp>


TEST_F(VariantTest, testConvertible)
{
    auto v0 = liter::convertible<std::string, char*>::value;
    EXPECT_EQ(true, v0);

    auto v1 = liter::convertible<char*, std::string>::value;
    EXPECT_EQ(false, v1);

    auto v2 = liter::convertible<int, const int>::value;
    EXPECT_EQ(true, v2);

    auto v3 = liter::convertible<const int, int>::value;
    EXPECT_EQ(true, v3);
}


TEST_F(VariantTest, testIntegerMax)
{
    auto v = liter::IntegerMax<1, 3, 5>::value;
    EXPECT_EQ(v, 5);

    v = liter::IntegerMax<4, 6, 3>::value;
    EXPECT_EQ(v, 6);

    v = liter::IntegerMax<500, 3, 47, 32, 45>::value;
    EXPECT_EQ(v, 500);
}
