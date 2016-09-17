#include "functionTraitsTest.h"

#include <gtest/gtest.h>

#include <type_traits>
#include <liter/function_traits.hpp>


void f0(){};
int f1(){
    return 0;
};

void g0(int i){};
int g1(double i){
    return i;
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


TEST_F(FunctionTraitsTest, testFn0Arg)
{
    auto num0 = liter::function_traits<decltype(f0)>::arity;
    EXPECT_EQ(0, num0);

    auto num1 = liter::function_traits<decltype(f1)>::arity;
    EXPECT_EQ(0, num1);
}


TEST_F(FunctionTraitsTest, testFn1Ret)
{
    using R0 = liter::function_traits<decltype(g0)>::return_type;
    auto v0 = std::is_same<void, R0>::value;

    EXPECT_EQ(v0, true);

    using R1 = liter::function_traits<decltype(g1)>::return_type;
    auto v1 = std::is_same<int, R1>::value;

    EXPECT_EQ(v1, true);
}


TEST_F(FunctionTraitsTest, testFn1Arg)
{
    auto num0 = liter::function_traits<decltype(g0)>::arity;
    EXPECT_EQ(1, num0);

    using arg00 = typename liter::function_traits<decltype(g0)>::template arg_type<0>;
    auto v00 = std::is_same<int, arg00>::value;
    EXPECT_EQ(true, v00);

    using arg01 = typename liter::function_traits<decltype(g0)>::template args<0>::type;
    auto v01 = std::is_same<int, arg01>::value;
    EXPECT_EQ(true, v01);

    auto num1 = liter::function_traits<decltype(g1)>::arity;
    EXPECT_EQ(1, num1);

    using arg10 = typename liter::function_traits<decltype(g1)>::template arg_type<0>;
    auto v10 = std::is_same<double, arg10>::value;
    EXPECT_EQ(true, v10);

    using arg11 = typename liter::function_traits<decltype(g1)>::template args<0>::type;
    auto v11 = std::is_same<double, arg11>::value;
    EXPECT_EQ(true, v11);
}
