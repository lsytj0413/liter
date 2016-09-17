#include "functionTraitsTest.h"

#include <gtest/gtest.h>

#include <type_traits>
#include <liter/function_traits.hpp>


void f(){};


TEST_F(FunctionTraitsTest, testFn0Ret)
{
    using R0 = liter::function_traits<decltype(f)>::return_type;
    auto v = std::is_same<void, R0>::value;

    EXPECT_EQ(v, true);
}
