#include "hierarchyGeneratorTest.h"

#include <iostream>
using namespace std;

#include <liter/loki/HierarchyGenerator.hpp>
using namespace liter::loki;

template <typename T>
struct ScatUnit
{
public:
    T i = 0;
};


TEST_F(HierarchyGenTest, testScatterZero)
{
    class Em {};

    using T1 = GenScatterHierarchy<Typelist<>, ScatUnit>;
    EXPECT_TRUE(sizeof(Em) == sizeof(T1));
}


TEST_F(HierarchyGenTest, testScatterOne)
{
    using T1 = GenScatterHierarchy<Typelist<int>, ScatUnit>;
    EXPECT_TRUE(sizeof(T1) == sizeof(int));

    auto v1 = T1();
    auto v = std::is_same<int, decltype(v1.i)>::value;
    EXPECT_EQ(v, true);

    v = std::is_same<detail::ScatterHierarchySign<Typelist<int>, ScatUnit>, typename T1::LeftBase>::value;
    EXPECT_EQ(v, true);

    using T2 = GenScatterHierarchy<int, ScatUnit>;
    EXPECT_TRUE(sizeof(T2) == sizeof(int));

    auto v2 = T2();
    v = std::is_same<int, decltype(v2.i)>::value;
    EXPECT_EQ(v, true);

    v = std::is_same<ScatUnit<int>, typename T2::LeftBase>::value;
    EXPECT_EQ(v, true);
}


TEST_F(HierarchyGenTest, testScatterMulti)
{
    using T1 = GenScatterHierarchy<Typelist<int, char>, ScatUnit>;
    using T2 = detail::ScatterHierarchySign<Typelist<int, char>, ScatUnit>;
    using T3 = GenScatterHierarchy<Typelist<char>, ScatUnit>;
    using T11 = ScatUnit<int>;

    auto v = std::is_same<T2, T1::LeftBase>::value;
    EXPECT_EQ(v, true);
    v = std::is_same<T3, T1::RightBase>::value;
    EXPECT_EQ(v, true);
    v = std::is_same<T2::LeftBase, T11>::value;
    EXPECT_EQ(v, true);

    using T4 = GenScatterHierarchy<Typelist<int, char, double>, ScatUnit>;
    using T5 = detail::ScatterHierarchySign<Typelist<int, char, double>, ScatUnit>;
    using T6 = GenScatterHierarchy<Typelist<char, double>, ScatUnit>;
    using T41 = ScatUnit<int>;

    v = std::is_same<T5, T4::LeftBase>::value;
    EXPECT_EQ(v, true);
    v = std::is_same<T6, T4::RightBase>::value;
    EXPECT_EQ(v, true);
    v = std::is_same<T41, T5::LeftBase>::value;
    EXPECT_EQ(v, true);
}


static char g_index = 0;

template <typename T>
struct ScatUnitField
{
public:
    T i;

    ScatUnitField() {
        i = g_index++;
    }
};


TEST_F(HierarchyGenTest, testFieldT)
{
    g_index = 0;

    using T1 = GenScatterHierarchy<Typelist<int, char, double>, ScatUnitField>;
    auto v = T1();

    auto v1 = Field<int>(v);
    auto b = std::is_same<int, decltype(v1.i)>::value;
    EXPECT_EQ(b, true);
    EXPECT_EQ(v1.i, 0);

    auto v2 = Field<char>(v);
    b = std::is_same<char, decltype(v2.i)>::value;
    EXPECT_EQ(b, true);
    EXPECT_EQ(v2.i, 1);

    auto v3 = Field<double>(v);
    b = std::is_same<double, decltype(v3.i)>::value;
    EXPECT_EQ(b, true);
    EXPECT_EQ(v3.i, 2);

    g_index = 0;
    using T2 = GenScatterHierarchy<Typelist<int, char, int>, ScatUnitField>;
    auto v4 = Field<int>(T2());
    b = std::is_same<int, decltype(v4.i)>::value;
    EXPECT_EQ(b, true);
}


TEST_F(HierarchyGenTest, testFieldI)
{
    g_index = 0;

    using T1 = GenScatterHierarchy<Typelist<int, char, double, int>, ScatUnitField>;
    auto v = T1();

    auto v1 = Field<0>(v);
    auto b = std::is_same<int, decltype(v1.i)>::value;
    EXPECT_EQ(b, true);
    EXPECT_EQ(v1.i, 0);

    auto v2 = Field<1>(v);
    b = std::is_same<char, decltype(v2.i)>::value;
    EXPECT_EQ(b, true);
    EXPECT_EQ(v2.i, 1);

    auto v3 = Field<2>(v);
    b = std::is_same<double, decltype(v3.i)>::value;
    EXPECT_EQ(b, true);
    EXPECT_EQ(v3.i, 2);

    auto v4 = Field<3>(v);
    b = std::is_same<int, decltype(v4.i)>::value;
    EXPECT_EQ(b, true);
    EXPECT_EQ(v4.i, 3);
}


TEST_F(HierarchyGenTest, testTuple)
{
    using T1 = GenScatterHierarchy<Typelist<int, char, double, int>, ScatUnit>;
    auto v = T1();

    auto sum = Field<0>(v).i + Field<1>(v).i + Field<2>(v).i + Field<3>(v).i;
    EXPECT_EQ(sum, 0);

    Field<0>(v).i = 10;
    Field<1>(v).i = 15;
    Field<2>(v).i = 30;
    Field<3>(v).i = 105;

    sum = Field<0>(v).i + Field<1>(v).i + Field<2>(v).i + Field<3>(v).i;
    EXPECT_EQ(sum, 160);
}


template <typename T, typename U>
struct ScatLinear {
    T i;
};

TEST_F(HierarchyGenTest, testLinearZero)
{
    class Em {};

    using T1 = GenLinearHierarchy<Typelist<>, ScatLinear>;
    EXPECT_EQ(sizeof(Em), sizeof(T1));
}

TEST_F(HierarchyGenTest, testLinearOne)
{
    using T1 = GenLinearHierarchy<Typelist<int>, ScatLinear>;
    EXPECT_EQ(sizeof(int), sizeof(T1));

    auto v = std::is_same<decltype(T1().i), int>::value;
    EXPECT_EQ(v, true);
}

template <typename T, typename U>
struct ScatLinearEvent : public U{
public:
    virtual T on_event(const T) = 0;
};

class EventObj : public GenLinearHierarchy<Typelist<int, double>, ScatLinearEvent>
{
public:
    virtual int on_event(const int) {return 100;};
    virtual double on_event(const double) {return 2.2;};
};

TEST_F(HierarchyGenTest, testLinearMulti)
{
    EventObj obj;
    EXPECT_EQ(obj.on_event(2), 100);
    EXPECT_EQ(2.2, obj.on_event(1.2));
}
