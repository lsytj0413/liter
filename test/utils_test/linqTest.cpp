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

    Person& operator=(const Person& p){
        age = p.age;
        name = p.name;
        address = p.address;

        return *this;
    };
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

    std::vector<int> v0 = {1, 2, 3, 3, 5, 5};

    auto f = liter::from(v0).count();
    EXPECT_EQ(f, 6);
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


TEST_F(LinqTest, testForEach)
{
    std::vector<Person> v = {
        {21, "a", "shanghai"},
        {22, "bb", "wuhan"},
        {21, "a", "zhuhai"}
    };

    int g = 0;
    liter::from(v).for_each([&g](const Person& p){
            g += p.age;
        });
    EXPECT_EQ(64, g);
}


TEST_F(LinqTest, testContains)
{
    std::vector<Person> v = {
        {21, "a", "shanghai"},
        {22, "bb", "wuhan"},
        {21, "a", "zhuhai"}
    };

    auto l = liter::from(v);
    auto f = l.contains([](const Person& p){
            return p.age == 23;
        });
    EXPECT_EQ(l.end(), f);

    f = l.contains([](const Person& p){
            return p.age == 22 || p.age == 21;
        });
    EXPECT_EQ(21, f->age);
}


TEST_F(LinqTest, testDistinct)
{
    std::vector<int> v0 = {1, 2, 3, 3, 5, 5};
    auto f = liter::from(v0).distinct().count();
    EXPECT_EQ(f, 4);

    // f = liter::from(v0).distinct([](int i, int j){
    //         return i == j;
    //     }).count();
    // EXPECT_EQ(f, 4);

    // std::vector<Person> v1 = {
    //     {21, "a", "shanghai"},
    //     {21, "bb", "wuhan"},
    //     {22, "a", "zhuhai"}
    // };

    // auto l = liter::from(v1);
    // f = l.distinct([](const Person& p1, const Person& p2){
    //         return p1.age == p2.age;
    //     }).count();

    // EXPECT_EQ(f, 2);
}


TEST_F(LinqTest, testAggregate)
{
    std::vector<int> v0 = {1, 2, 3, 3, 5, 5};

    auto l = liter::from(v0);
    auto f = l.aggregate([](int i, int j){
            return i + j;
        });

    EXPECT_EQ(f, 19);
}


TEST_F(LinqTest, testSum)
{
    std::vector<int> v0 = {1, 2, 3, 3, 5, 5};

    auto f = liter::from(v0).sum();
    EXPECT_EQ(f, 19);
}


TEST_F(LinqTest, testAverage)
{
    std::vector<int> v0 = {1, 2, 3, 3, 5, 5};

    auto f = liter::from(v0).average();
    EXPECT_EQ(f, 3);
}


TEST_F(LinqTest, testMin)
{
    std::vector<int> v0 = {1, 2, 3, 3, 5, 5};

    auto f = liter::from(v0).min();
    EXPECT_EQ(f, 1);

    std::vector<Person> v1 = {
        {21, "a", "shanghai"},
        {21, "bb", "wuhan"},
        {22, "a", "zhuhai"}
    };

    auto l = liter::from(v1);
    auto f1 = l.min([](const Person& p1, const Person& p2){
            return p1.age < p2.age;
        });

    EXPECT_EQ(f1.age, 21);
}


TEST_F(LinqTest, testMax)
{
    std::vector<int> v0 = {1, 2, 3, 3, 5, 5};

    auto f = liter::from(v0).max();
    EXPECT_EQ(f, 5);

    std::vector<Person> v1 = {
        {21, "a", "shanghai"},
        {21, "bb", "wuhan"},
        {22, "a", "zhuhai"}
    };

    auto l = liter::from(v1);
    auto f1 = l.max([](const Person& p1, const Person& p2){
            return p1.age < p2.age;
        });

    EXPECT_EQ(f1.age, 22);
}


TEST_F(LinqTest, testMinmax)
{
    std::vector<int> v0 = {1, 2, 3, 3, 5, 5};

    auto f = liter::from(v0).minmax();
    EXPECT_EQ((f.first), 1);
    EXPECT_EQ((f.second), 5);

    std::vector<Person> v1 = {
        {21, "a", "shanghai"},
        {21, "bb", "wuhan"},
        {22, "a", "zhuhai"}
    };

    auto l = liter::from(v1);
    auto f1 = l.minmax([](const Person& p1, const Person& p2){
            return p1.age < p2.age;
        });

    EXPECT_EQ(f1.first.age, 21);
    EXPECT_EQ(f1.second.age, 22);
}
