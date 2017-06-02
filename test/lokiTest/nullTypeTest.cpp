#include "nullTypeTest.h"

#include <liter/loki/null_type.hpp>


TEST_F(NullTypeTest, test)
{
    liter::loki::NullType t;
    EXPECT_TRUE(sizeof(t) > 0);
}
