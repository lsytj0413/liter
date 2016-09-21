#include "variantTest.h"

#include <gtest/gtest.h>
#include <string>

#include <liter/variant.hpp>


TEST_F(VariantTest, testConvertible)
{
    auto v0 = liter::convertible<std::string, char*>::value;
    EXPECT_EQ(true, v0);

    auto v1 = liter::convertible<char*, std::string>::value;
    EXPECT_EQ(false, v1);

    auto v2 = liter::convertible<int, const int>::value;
    EXPECT_EQ(true, v2);

    auto v3 = liter::convertible<const int, int>::value;
    EXPECT_EQ(true, v3);
}


TEST_F(VariantTest, testConvType)
{
    using T0 = typename liter::ConvType<char*, std::string>::type;
    auto v0 = std::is_same<T0, std::string>::value;
    EXPECT_EQ(v0, true);
}


TEST_F(VariantTest, testIntegerMax)
{
    auto v = liter::IntegerMax<1, 3, 5>::value;
    EXPECT_EQ(v, 5);

    v = liter::IntegerMax<4, 6, 3>::value;
    EXPECT_EQ(v, 6);

    v = liter::IntegerMax<500, 3, 47, 32, 45>::value;
    EXPECT_EQ(v, 500);
}


TEST_F(VariantTest, testContains)
{
    auto v = liter::Contains<int, double, int, char, std::string>::value;
    EXPECT_EQ(true, v);

    v = liter::Contains<int, double, char, std::string>::value;
    EXPECT_EQ(false, v);

    v = liter::Contains<int>::value;
    EXPECT_EQ(false, v);

    v = liter::Contains<typename std::decay<char>::type, char>::value;
    EXPECT_EQ(true, v);
}


TEST_F(VariantTest, testIndexOf)
{
    auto v0 = liter::IndexOf<int, int, double, int>::value;
    EXPECT_EQ(0, v0);

    auto v1 = liter::IndexOf<int, char, std::string, const int>::value;
    EXPECT_EQ(-1, v1);

    auto v2 = liter::IndexOf<int, double, int>::value;
    EXPECT_EQ(v2, 1);

    auto v3 = liter::IndexOf<int, char, double, int, char>::value;
    EXPECT_EQ(v3, 2);
}


TEST_F(VariantTest, testAt)
{
    using T0 = typename liter::At<0, int, double, char>::type;
    auto v0 = std::is_same<int, T0>::value;
    EXPECT_EQ(true, v0);

    using T1 = typename liter::At<2, int, double, char>::type;
    auto v1 = std::is_same<char, T1>::value;
    EXPECT_EQ(true, v1);

    using T2 = typename liter::At<1, int, double, char>::type;
    auto v2 = std::is_same<double, T2>::value;
    EXPECT_EQ(true, v2);

}


TEST_F(VariantTest, testVariantIs)
{
    using T0 = liter::variant<int, char>;
    auto v0 = T0();
    EXPECT_EQ(false, v0.is<int>());
    EXPECT_EQ(false, v0.is<char>());

    v0 = T0(10);
    EXPECT_EQ(true, v0.is<int>());
    EXPECT_EQ(false, v0.is<char>());

    v0 = T0('a');
    EXPECT_EQ(false, v0.is<int>());
    EXPECT_EQ(true, v0.is<char>());

    v0 = T0();
    EXPECT_EQ(false, v0.is<int>());
    EXPECT_EQ(false, v0.is<char>());
}

TEST_F(VariantTest, testVariantEmpty)
{
    using T0 = liter::variant<int, char>;
    auto v0 = T0();
    EXPECT_EQ(true, v0.empty());

    v0 = T0('a');
    EXPECT_EQ(false, v0.empty());

    v0 = T0();
    EXPECT_EQ(true, v0.empty());
}


TEST_F(VariantTest, testVariantGet)
{
    using T0 = liter::variant<int, char>;
    auto v0 = T0();

    bool bad = false;
    try{
        v0.get<int>();
        bad = false;
    }
    catch(std::bad_cast& e){
        bad = true;
    }
    EXPECT_EQ(bad, true);

    try{
        v0.get<char>();
        bad = false;
    }
    catch(std::bad_cast& e){
        bad = true;
    }
    EXPECT_EQ(bad, true);

    v0 = T0(10);
    EXPECT_EQ(10, v0.get<int>());

    v0 = T0('a');
    EXPECT_EQ('a', v0.get<char>());
}


TEST_F(VariantTest, testIndex)
{
    using T0 = liter::variant<int, char>;
    auto v0 = T0();

    EXPECT_EQ(v0.index<int>(), 0);
    EXPECT_EQ(v0.index<char>(), 1);
}


TEST_F(VariantTest, testVisit)
{
    using T0 = liter::variant<int>;
    auto v0 = T0(1);

    int g = 0;
    v0.visit([&](int i){
            g = i;
        });

    EXPECT_EQ(g, 1);

    using T1 = liter::variant<int, char>;
    auto v1 = T1(10);

    v1.visit([&](int i){
            g = i;
        });
    EXPECT_EQ(g, 10);

    g = 0;
    v1 = T1('c');
    char c = 'a';
    v1.visit([&](int i){
            g = i;
        },
        [&](char i){
            c = i;
        });

    EXPECT_EQ(g, 0);
    EXPECT_EQ(c, 'c');
}

TEST_F(VariantTest, testEqual)
{
    using T0 = liter::variant<int, char>;
    auto v0 = T0();
    auto v1 = T0();

    EXPECT_EQ(v0, v1);

    v0 = T0(10);
    v1 = T0('c');
    if (v0 == v1){
        EXPECT_EQ(1, 0);
    }

    v1 = T0(15);
    if (v0 == v1){
        EXPECT_EQ(1, 0);
    }

    v1 = T0(10);
    if (!(v0 == v1)){
        EXPECT_EQ(1, 0);
    }
}


TEST_F(VariantTest, testCons)
{
    liter::variant<std::string> v{"123"};
}
