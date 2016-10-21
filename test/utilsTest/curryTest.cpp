#include "curryTest.h"

#include <gtest/gtest.h>
#include <liter/curry.hpp>
#include <liter/function_traits.hpp>


int test_f0(){
    return 10;
};

TEST_F(CurryTest, testArgs0)
{
    auto v = liter::curry(test_f0);
    EXPECT_EQ(10, v());
}

// int test_f1(int i){
//     return i;
// };

// TEST_F(CurryTest, testArgs1)
// {
//     auto v = liter::curry(test_f1);
//     EXPECT_EQ(1, v(1));
// }
