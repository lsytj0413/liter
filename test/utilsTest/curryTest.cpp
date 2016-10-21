#include "curryTest.h"

#include <gtest/gtest.h>
#include <liter/curry.hpp>
#include <liter/function_traits.hpp>


// int test_f0(){
//     return 10;
// };

// TEST_F(CurryTest, testArgs0)
// {
//     auto v = liter::curry(test_f0);
//     EXPECT_EQ(10, v());
// }

int test_f1(int i) {
    return i;
};

TEST_F(CurryTest, testArgs1)
{
    auto v = liter::curry(test_f1);
    EXPECT_EQ(1, v(1));
}

int test_f2(int i, int j){
    return i + j;
};

TEST_F(CurryTest, testArgs2)
{
    auto v = liter::curry(test_f2);
    EXPECT_EQ(11, v(1)(10));
}


int test_f4(int i, char c, float f, double d) {
    return i + c + f + d;
}

TEST_F(CurryTest, testArgs4) {
    auto v = liter::curry(test_f4);
    EXPECT_EQ(v(2)(1)(1.2)(4.7), 8);
}
