#include "utilTest.h"

#include <liter/loki/util.hpp>


TEST_F(UtilTest, test)
{
    liter::loki::NullType t;
    EXPECT_TRUE(sizeof(t) > 0);
}
