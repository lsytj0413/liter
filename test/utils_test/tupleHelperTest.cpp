#include "tupleHelperTest.h"

#include <gtest/gtest.h>
#include <liter/tuple_helper.hpp>
#include <string>


TEST_F(TupleHelperTest, testFindIndex)
{
    auto v0 = std::tuple<int, char, double>(1, 'a', 1.2);
    int index = liter::find_index(v0, 1);
    EXPECT_EQ(index, 0);

    index = liter::find_index(v0, 'a');
    EXPECT_EQ(index, 1);

    index = liter::find_index(v0, 1.2);
    EXPECT_EQ(index, 2);

    index = liter::find_index(v0, 1.23);
    EXPECT_EQ(index, -1);

    index = liter::find_index(v0, 'c');
    EXPECT_EQ(index, -1);

    index = liter::find_index(v0, 2);
    EXPECT_EQ(index, -1);

    index = liter::find_index(v0, "abd");
    EXPECT_EQ(index, -1);
}


// TEST_F(TupleHelperTest, testTpForEach)
// {
//     auto v0 = std::tuple<double, double, double>(1.5, 2.7, 1.2);
//     double r = 0;

//     liter::tp_for_each([&](double d){
//             r += d;
//         }, v0);
//     EXPECT_EQ(r, 5.4);
// }


TEST_F(TupleHelperTest, testReverse)
{
    auto v0 = std::tuple<double, int, char>(1.2, 5, 'a');
    auto v1 = liter::reverse(v0);

    auto i0 = std::is_same<decltype(v1), std::tuple<char, int, double>>::value;
    EXPECT_EQ(true, i0);

    EXPECT_EQ(std::get<0>(v1), 'a');
    EXPECT_EQ(std::get<1>(v1), 5);
    EXPECT_EQ(std::get<2>(v1), 1.2);
}


TEST_F(TupleHelperTest, testApply)
{
    auto v0 = std::tuple<double, double, double>(1.5, 2.7, 1.2);
    double r = 0;

    liter::apply([&](double d){
            r += d;
        }, v0);
    EXPECT_EQ(r, 5.4);
}
