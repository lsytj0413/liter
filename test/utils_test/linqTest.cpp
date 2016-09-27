#include "linqTest.h"

#include <gtest/gtest.h>

#include <liter/linq.hpp>
#include <vector>
#include <string>
#include <iostream>

#include <cxxabi.h>

struct Person
{
    int age;
    std::string name;
    std::string address;

    // Person(int a, std::string& b, std::string& c){
    //     age = a;
    //     name = b;
    //     address = c;
    // };
};


TEST_F(LinqTest, testWhere)
{
    std::vector<Person> v = {
        {21, "a", "shanghai"},
        {22, "bb", "wuhan"},
        {21, "a", "zhuhai"}
    };

    auto p = liter::from(v).where([](const Person& p){
            return p.age > 21;
        }).begin();

    EXPECT_EQ(22, p->age);
    EXPECT_EQ("bb", p->name);
}


// TEST_F(LinqTest, testSelect)
// {
//     std::vector<Person> v = {
//         {21, "a", "shanghai"},
//         {22, "bb", "wuhan"},
//         {21, "a", "zhuhai"}
//     };

//     auto p = liter::from(v).select([](const Person& p){
//             return p.age > 21;
//         });
//     auto v1 = p.begin().base();

//     int status = 0;
//     std::cout << abi::__cxa_demangle(typeid(v1).name(), 0, 0, &status) << std::endl;
//     // std::cout << typeid(p).name() << std::endl;
//     // EXPECT_EQ(22, p.first(auto) first( const F &f ));
//     // EXPECT_EQ("bb", p->name);
// }


TEST_F(LinqTest, testEmpty)
{
    std::vector<Person> v = {
        // {21, "a", "shanghai"},
        // {22, "bb", "wuhan"},
        // {21, "a", "zhuhai"}
    };

    auto v0 = liter::from(v).empty();
    EXPECT_EQ(true, v0);

    Person p;
    v.emplace_back(p);

    auto v1 = liter::from(v).empty();
    EXPECT_EQ(false, v1);
}


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


TEST_F(LinqTest, testReverse)
{
    std::vector<Person> v = {
        {21, "a", "shanghai"},
        {22, "bb", "wuhan"},
        {21, "a", "zhuhai"}
    };

    auto l = liter::from(v).reverse();
    auto v1 = l.begin();

    EXPECT_EQ("zhuhai", v1->address);
}


TEST_F(LinqTest, testFirst)
{
    std::vector<Person> v = {
        {21, "a", "shanghai"},
        {22, "bb", "wuhan"},
        {21, "a", "zhuhai"}
    };

    auto f = liter::from(v).first([](const Person& p){
            return p.age > 21;
        });
    EXPECT_EQ(22, f->age);
}


TEST_F(LinqTest, testLast)
{
    std::vector<Person> v = {
        {21, "a", "shanghai"},
        {22, "bb", "wuhan"},
        {21, "a", "zhuhai"}
    };

    auto f = liter::from(v).last([](const Person& p){
            return p.age == 21;
        });
    EXPECT_EQ("zhuhai", f->address);
}


TEST_F(LinqTest, testAny)
{
    std::vector<Person> v = {
        {21, "a", "shanghai"},
        {22, "bb", "wuhan"},
        {21, "a", "zhuhai"}
    };

    auto f = liter::from(v).any([](const Person& p){
            return p.age == 22;
        });
    EXPECT_EQ(true, f);

    f = liter::from(v).any([](const Person& p){
            return p.age == 23;
        });
    EXPECT_EQ(false, f);
}


TEST_F(LinqTest, testAll)
{
    std::vector<Person> v = {
        {21, "a", "shanghai"},
        {22, "bb", "wuhan"},
        {21, "a", "zhuhai"}
    };

    auto f = liter::from(v).all([](const Person& p){
            return p.age == 22;
        });
    EXPECT_EQ(false, f);

    f = liter::from(v).all([](const Person& p){
            return p.age == 22 || p.age == 21;
        });
    EXPECT_EQ(true, f);
}
