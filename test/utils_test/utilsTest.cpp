#include "utilsTest.h"

#include <gtest/gtest.h>
#include <liter/utils.h>


TEST_F(UtilsTest, testVsumOne)
{
    auto v = liter::variadic_sum(1);

    EXPECT_EQ(v, 1);
}
