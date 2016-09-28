#include "optionalTest.h"

#include <gtest/gtest.h>

#include <liter/optional.hpp>


TEST_F(OptionalTest, testNullp)
{
    liter::optional<int> op;
    EXPECT_EQ(op.init_p(), false);

    op.emplace(1);
    EXPECT_EQ(op.init_p(), true);
}


TEST_F(OptionalTest, testValue)
{
    liter::optional<int> op;
    bool bad = false;
    int v = 0;

    try{
        v = *op;
        bad = false;
    }
    catch(std::logic_error& e){
        bad = true;
    }
    EXPECT_EQ(bad, true);

    op.emplace(1);
    v = *op;
    EXPECT_EQ(v, 1);

    liter::optional<int> op1(20);
    EXPECT_EQ(*op1, 20);
}
