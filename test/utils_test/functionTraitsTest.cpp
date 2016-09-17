#include "functionTraitsTest.h"

#include <gtest/gtest.h>

#include <type_traits>
#include <liter/function_traits.hpp>


void f0(){};
int f1(){
    return 0;
};


TEST_F(FunctionTraitsTest, testFn0Ret)
{
    using R0 = liter::function_traits<decltype(f0)>::return_type;
    auto v0 = std::is_same<void, R0>::value;

    EXPECT_EQ(v0, true);

    using R1 = liter::function_traits<decltype(f1)>::return_type;
    auto v1 = std::is_same<int, R1>::value;

    EXPECT_EQ(v1, true);
}
