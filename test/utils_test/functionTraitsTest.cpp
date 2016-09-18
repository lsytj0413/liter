#include "functionTraitsTest.h"

#include <gtest/gtest.h>

#include <functional>
#include <type_traits>
#include <liter/function_traits.hpp>

// #include <iostream>
// #include <cxxabi.h>


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


TEST_F(FunctionTraitsTest, testFn0Arg)
{
    auto num0 = liter::function_traits<decltype(f0)>::arity;
    EXPECT_EQ(0, num0);

    auto num1 = liter::function_traits<decltype(f1)>::arity;
    EXPECT_EQ(0, num1);
}


void g0(int i){};
int g1(double i){
    return i;
};


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


double h0(int i, double j, char c){
    return j;
};

char h1(int j, char c){
    return c;
};


TEST_F(FunctionTraitsTest, testFn2Ret)
{
    using R0 = liter::function_traits<decltype(h0)>::return_type;
    auto v0 = std::is_same<double, R0>::value;

    EXPECT_EQ(v0, true);

    using R1 = liter::function_traits<decltype(h1)>::return_type;
    auto v1 = std::is_same<char, R1>::value;

    EXPECT_EQ(v1, true);
}


TEST_F(FunctionTraitsTest, testFn2Arg)
{
    auto num0 = liter::function_traits<decltype(h0)>::arity;
    EXPECT_EQ(3, num0);

    using arg00 = typename liter::function_traits<decltype(h0)>::template arg_type<0>;
    auto v00 = std::is_same<int, arg00>::value;
    EXPECT_EQ(true, v00);

    using arg01 = typename liter::function_traits<decltype(h0)>::template args<1>::type;
    auto v01 = std::is_same<double, arg01>::value;
    EXPECT_EQ(true, v01);

    using arg02 = typename liter::function_traits<decltype(h0)>::template args<2>::type;
    auto v02 = std::is_same<char, arg02>::value;
    EXPECT_EQ(true, v02);

    auto num1 = liter::function_traits<decltype(h1)>::arity;
    EXPECT_EQ(2, num1);

    using arg10 = typename liter::function_traits<decltype(h1)>::template arg_type<0>;
    auto v10 = std::is_same<int, arg10>::value;
    EXPECT_EQ(true, v10);

    using arg11 = typename liter::function_traits<decltype(h1)>::template args<1>::type;
    auto v11 = std::is_same<char, arg11>::value;
    EXPECT_EQ(true, v11);
}


TEST_F(FunctionTraitsTest, testStdFunction)
{
    using F0 = liter::function_traits<std::function<int()>>;

    auto num0 = F0::arity;
    EXPECT_EQ(0, num0);

    using ret0 = typename F0::return_type;
    auto v0 = std::is_same<int, ret0>::value;
    EXPECT_EQ(true, v0);

    using F1 = liter::function_traits<std::function<int(double)>>;

    auto num1 = F1::arity;
    EXPECT_EQ(1, num1);

    using ret1 = typename F1::return_type;
    auto v1 = std::is_same<int, ret1>::value;
    EXPECT_EQ(true, v1);

    using arg0 = typename F1::template args<0>::type;
    auto v2 = std::is_same<double, arg0>::value;
    EXPECT_EQ(true, v2);

    using F2 = liter::function_traits<std::function<double(int, char)>>;

    auto num2 = F2::arity;
    EXPECT_EQ(2, num2);

    using ret2 = typename F2::return_type;
    auto v3 = std::is_same<double, ret2>::value;
    EXPECT_EQ(true, v3);

    using arg1 = typename F2::template args<0>::type;
    auto v4 = std::is_same<int, arg1>::value;
    EXPECT_EQ(true, v4);

    using arg2 = typename F2::template args<1>::type;
    auto v5 = std::is_same<char, arg2>::value;
    EXPECT_EQ(true, v5);
}


TEST_F(FunctionTraitsTest, testFunction)
{
    using F0 = liter::function_traits<decltype(&f0)>;

    auto num0 = F0::arity;
    EXPECT_EQ(0, num0);

    using ret0 = typename F0::return_type;
    auto v = std::is_same<void, ret0>::value;
    EXPECT_EQ(true, v);

    using F1 = liter::function_traits<decltype(&h0)>;

    auto num1 = F1::arity;
    EXPECT_EQ(3, num1);

    using ret1 = typename F1::return_type;
    v = std::is_same<double, ret1>::value;
    EXPECT_EQ(true, v);

    using arg00 = typename F1::template args<0>::type;
    v = std::is_same<int, arg00>::value;
    EXPECT_EQ(true, v);

    using arg01 = typename F1::template args<1>::type;
    v = std::is_same<double, arg01>::value;
    EXPECT_EQ(true, v);

    using arg02 = typename F1::template args<2>::type;
    v = std::is_same<char, arg02>::value;
    EXPECT_EQ(true, v);
}


class TestClassMemberFn
{
public:
    int f(char, double){
        return 1;
    };
};

TEST_F(FunctionTraitsTest, testClassMemberFn)
{
    using F = int(TestClassMemberFn::*)(char, double);
    using F0 = liter::function_traits<F>;

    auto num0 = F0::arity;
    EXPECT_EQ(2, num0);

    using ret0 = typename F0::return_type;
    auto v = std::is_same<int, ret0>::value;
    EXPECT_EQ(true, v);

    using arg00 = typename F0::template args<0>::type;
    v = std::is_same<char, arg00>::value;
    EXPECT_EQ(true, v);

    using arg01 = typename F0::template args<1>::type;
    v = std::is_same<double, arg01>::value;
    EXPECT_EQ(true, v);
}


class TestCallable
{
public:
    int operator()(){
        return 0;
    };
};


TEST_F(FunctionTraitsTest, testCallable)
{
    using F0 = liter::function_traits<TestCallable>;

    auto num0 = F0::arity;
    EXPECT_EQ(0, num0);

    using ret0 = typename F0::return_type;
    auto v = std::is_same<int, ret0>::value;
    EXPECT_EQ(true, v);
}


TEST_F(FunctionTraitsTest, testLambdaToFunction)
{
    auto f = []()->int{
        return 0;
    };
    using L0 = decltype(f);
    // using F0 = typename std::result_of<liter::to_function<L0>>::type;

    auto r0 = liter::to_function(f);
    using F0 = liter::function_traits<decltype(r0)>;

    // int status = 0;
    // std::cout << abi::__cxa_demangle(typeid(F0).name(), 0, 0, &status) << std::endl;

    auto num0 = F0::arity;
    EXPECT_EQ(0, num0);

    auto v = std::is_same<int, F0::return_type>::value;
    EXPECT_EQ(true, v);


    auto f1 = [&](int, double) -> char {
        return 'c';
    };
    auto r1 = liter::to_function(f1);
    using F1 = liter::function_traits<decltype(r1)>;

    auto num1 = F1::arity;
    EXPECT_EQ(2, num1);

    v = std::is_same<char, F1::return_type>::value;
    EXPECT_EQ(true, v);

    using arg00 = typename F1::template args<0>::type;
    v = std::is_same<int, arg00>::value;
    EXPECT_EQ(true, v);

    using arg01 = typename F1::template args<1>::type;
    v = std::is_same<double, arg01>::value;
    EXPECT_EQ(true, v);
}


TEST_F(FunctionTraitsTest, testLambdaToFunctionPointer)
{
    auto f0 = []() -> int{
        return 12;
    };

    auto r0 = liter::to_function_pointer(f0);
    using P0 = int(*)();
    auto v = std::is_same<P0, decltype(r0)>::value;
    EXPECT_EQ(true, v);

    auto f1 = [&](int, double) -> char {
        return 'c';
    };
    auto r1 = liter::to_function_pointer(f1);
    using P1 = char(*)(int, double);
    v = std::is_same<P1, decltype(r1)>::value;
    EXPECT_EQ(true, v);
}
