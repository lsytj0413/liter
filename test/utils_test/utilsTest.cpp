#include "utilsTest.h"

#include <gtest/gtest.h>
#include <liter/utils.h>


TEST_F(UtilsTest, testVsumOne)
{
    auto v = liter::variadic_sum(1);
    EXPECT_EQ(v, 1);

    auto v1 = liter::variadic_sum(2.1);
    EXPECT_FLOAT_EQ(v1, 2.1);

    auto v2 = liter::variadic_sum('c');
    EXPECT_EQ(v2, 'c');
}
