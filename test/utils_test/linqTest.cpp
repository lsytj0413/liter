#include "linqTest.h"

#include <gtest/gtest.h>

#include <liter/linq.hpp>
#include <vector>
#include <string>


struct Person
{
    int age;
    std::string name;
    std::string address;
};


TEST_F(LinqTest, testCount)
{
    std::vector<Person> v = {
        {21, "a", "shanghai"},
        {22, "bb", "wuhan"},
        {21, "a", "zhuhai"}
    };

    auto count = liter::from(v).where([](const Person& p){
            return p.age > 21;
        }).count();
    EXPECT_EQ(1, count);
}
