#include "linqTest.h"

#include <gtest/gtest.h>

#include <liter/linq.hpp>
#include <vector>
#include <map>
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


TEST_F(LinqTest, testSelect)
{
    std::vector<Person> v = {
        {21, "a", "shanghai"},
        {22, "bb", "wuhan"},
        {21, "a", "zhuhai"}
    };

    auto p = liter::from(v).select([](const Person& p){
            return p.age > 21;
        });
    auto v1 = p.begin();

    // int status = 0;
    // std::cout << abi::__cxa_demangle(typeid(v1).name(), 0, 0, &status) << std::endl;

    EXPECT_EQ(false, *v1);

    v1++;
    EXPECT_EQ(true, *v1);
}


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
    auto f = liter::from(v0).where([](int i){return true;}).distinct().count();
    EXPECT_EQ(f, 4);

    // auto f1 = liter::from(v0).distinct([](auto i, auto j){
    //         return i == j;
    //     }).count();

    // EXPECT_EQ(f1, 4);

    // unique(boost::iterator_range<std::vector<int>::iterator>(v0), [](int i, int j){
    //         return i == j;
    //     });


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


TEST_F(LinqTest, testElemantAt)
{
    std::vector<int> v0 = {1, 2, 3, 3, 5, 5};

    auto f = liter::from(v0).element_at(1);
    EXPECT_EQ(*f, 2);

    f = liter::from(v0).element_at(3);
    EXPECT_EQ(*f, 3);

    f = liter::from(v0).element_at(2);
    EXPECT_EQ(*f, 3);

    f = liter::from(v0).element_at(5);
    EXPECT_EQ(*f, 5);
}


TEST_F(LinqTest, testKeys)
{
    std::map<int, int> v0 = {
        {1, 2},
        {3, 4}
    };

    auto f = liter::from(v0).keys();
    auto v1 = f.element_at(0).base()->first;
    // int status = 0;
    // std::cout << abi::__cxa_demangle(typeid(v1).name(), 0, 0, &status) << std::endl;
    EXPECT_EQ(v1, 1);
}


TEST_F(LinqTest, testValues)
{
    std::map<int, int> v0 = {
        {1, 2},
        {3, 4}
    };

    auto f = liter::from(v0).values();
    auto v1 = f.element_at(0).base()->second;
    // int status = 0;
    // std::cout << abi::__cxa_demangle(typeid(v1).name(), 0, 0, &status) << std::endl;
    EXPECT_EQ(v1, 2);
}


TEST_F(LinqTest, testTake)
{
    std::vector<int> v0 = {1, 2, 3, 3, 5, 5};

    auto f1 = liter::from(v0).take(2);
    auto v1 = f1.element_at(1).base();
    EXPECT_EQ(*v1, 2);

    auto f2 = liter::from(v0).take(2, 4);
    auto c2 = f2.count();
    EXPECT_EQ(c2, 2);
    auto v2 = f2.element_at(1).base();
    EXPECT_EQ(*v2, 3);
}


TEST_F(LinqTest, testToVector)
{
    std::vector<int> v0 = {1, 2, 3, 3, 5, 5};

    auto f1 = liter::from(v0).take(2).to_vector();

    EXPECT_EQ(2, f1.size());
    EXPECT_EQ(1, f1.at(0));
    EXPECT_EQ(2, f1.at(1));
}


TEST_F(LinqTest, testTakeWhile)
{
    std::vector<int> v0 = {1, 2, 3, 3, 4, 5, 2};

    auto f1 = liter::from(v0).take_while([](int i){
            return i > 3;
        });

    auto f2 = f1.element_at(3).base();
    EXPECT_EQ(*f2, 3);
    EXPECT_EQ(f1.count(), 4);

    std::vector<int> v1 = {1, 2, 3, 3};
    auto b = f1.equals(liter::from(v1));
    EXPECT_EQ(b, true);
}


TEST_F(LinqTest, testSkip)
{
    std::vector<int> v0 = {1, 2, 3, 3, 4, 5, 2};

    auto f1 = liter::from(v0).skip(3);

    std::vector<int> v1 = {3, 4, 5, 2};
    auto b = f1.equals(liter::from(v1));
    EXPECT_EQ(b, true);
}


TEST_F(LinqTest, testSkipWhile)
{
    std::vector<int> v0 = {1, 2, 3, 3, 4, 5, 2};

    auto f1 = liter::from(v0).skip_while([](int i){
            return i < 3;
        });

    std::vector<int> v1 = {3, 3, 4, 5, 2};
    auto b = f1.equals(liter::from(v1));
    EXPECT_EQ(b, true);
}


TEST_F(LinqTest, testStep)
{
    std::vector<int> v0 = {1, 2, 3, 3, 4, 5, 2};

    auto f1 = liter::from(v0).step(3);

    // int status = 0;
    // std::cout << abi::__cxa_demangle(typeid(f1).name(), 0, 0, &status) << std::endl;

    EXPECT_EQ(f1.size(), 3);

    auto j = 0;
    auto it = f1.begin();
    for(int i = 0; i < v0.size(); i = i + 3){
        EXPECT_EQ(v0[i], *it);
        ++it;
    }
}


TEST_F(LinqTest, testIndirect)
{
    std::vector<int*> v0 = {
        new int(1),
        new int(2)
    };

    auto f1 = liter::from(v0).indirect();

    std::vector<int> v1 = {1, 2};
    auto b = f1.equals(liter::from(v1));
    EXPECT_EQ(b, true);
}


TEST_F(LinqTest, testConcat)
{
    std::vector<int> v1 = {1, 2};
    std::vector<int> v2 = {3, 4, 5};

    auto f1 = liter::from(v1).concat(v2);

    EXPECT_EQ(f1.count(), 5);

    EXPECT_EQ(4, *(f1.element_at(3)));
    // std::vector<int> v3 = {1, 2, 3, 4, 5};
    // auto b = f1.equals(liter::from(v1));
    // EXPECT_EQ(b, true);
}


TEST_F(LinqTest, testExcept)
{
    std::vector<int> v1 = {1, 2};
    std::vector<int> v2 = {2, 3, 5};

    std::vector<int> v3;
    liter::from(v1).except(v2, v3);

    EXPECT_EQ(v3.size(), 1);
    EXPECT_EQ(v3[0], 1);
}


TEST_F(LinqTest, testIncludes)
{
    std::vector<int> v1 = {1, 2};
    std::vector<int> v2 = {2, 3, 5};

    auto f1 = liter::from(v1).includes(v2);
    EXPECT_EQ(f1, false);

    v1 = {2, 3, 5, 7, 100};
    v2 = {3, 5};

    f1 = liter::from(v1).includes(v2);
    EXPECT_EQ(f1, true);
}


TEST_F(LinqTest, testGroupBy)
{
    std::vector<int> v1 = {2, 3, 5, 7, 100};

    auto f1 = liter::from(v1).group_by([](int i){
            if (i <= 5){
                return 0;
            }

            return 1;
        });
    EXPECT_EQ(f1.size(), 5);
    EXPECT_EQ(f1.count(0), 3);
    EXPECT_EQ(f1.count(1), 2);

    auto f2 = liter::from(v1).group_by([](int i){
            if (i < 5){
                return 0;
            }

            return 1;
        }, [](int i){
            return 2*i;
        });

    EXPECT_EQ(f2.count(0), 2);
    EXPECT_EQ(f2.count(1), 3);

    auto it = f2.lower_bound(0);
    EXPECT_EQ(it->second, 4);

    it = f2.lower_bound(1);
    EXPECT_EQ(it->second, 10);
}


TEST_F(LinqTest, testCast)
{
    std::vector<int> v1 = {2, 3, 5, 7, 100};

    auto f1 = liter::from(v1).cast<int>();
    EXPECT_EQ(f1.count(), 5);
}


TEST_F(LinqTest, testZip)
{
    std::vector<int> v1 = {2, 3, 5, 7, 100};
    std::vector<int> v2 = {4, 6, 10, 17};

    auto f1 = liter::zip(v1, v2);
    EXPECT_EQ(f1.size(), 5);

    auto it = f1.begin();
    EXPECT_EQ((*it).get<0>(), 2);
    EXPECT_EQ((*it).get<1>(), 4);

    ++it;
    EXPECT_EQ((*it).get<0>(), 3);
    EXPECT_EQ((*it).get<1>(), 6);

    ++it;
    EXPECT_EQ((*it).get<0>(), 5);
    EXPECT_EQ((*it).get<1>(), 10);

    ++it;
    EXPECT_EQ((*it).get<0>(), 7);
    EXPECT_EQ((*it).get<1>(), 17);

    ++it;
    EXPECT_EQ((*it).get<0>(), 100);
    EXPECT_EQ((*it).get<1>(), 0);
}
