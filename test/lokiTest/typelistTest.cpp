#include "typelistTest.h"

#include <iostream>
using namespace std;

#include <liter/loki/typelist.hpp>
using namespace liter::loki;


TEST_F(TypelistTest, testLength)
{
    using T1 = Typelist<>;
    auto v = Length<T1>::value;
    EXPECT_EQ(v, 0);

    using T2 = Typelist<int>;
    v = Length<T2>::value;
    EXPECT_EQ(v, 1);

    using T3 = Typelist<int, char, double>;
    v = Length<T3>::value;
    EXPECT_EQ(v, 3);
}

TEST_F(TypelistTest, testTypeAt)
{
    using T3 = Typelist<int, char, double>;
    auto v = std::is_same<int, typename TypeAt<T3, 0>::Result>::value;
    EXPECT_EQ(v, true);

    v = std::is_same<char, typename TypeAt<T3, 1>::Result>::value;
    EXPECT_EQ(v, true);

    v = std::is_same<double, typename TypeAt<T3, 2>::Result>::value;
    EXPECT_EQ(v, true);
}

TEST_F(TypelistTest, testTypeAtNonStrict)
{
    using T3 = Typelist<int, char, double>;
    auto v = std::is_same<int, typename TypeAtNonStrict<T3, 0>::Result>::value;
    EXPECT_EQ(v, true);

    v = std::is_same<char, typename TypeAtNonStrict<T3, 1>::Result>::value;
    EXPECT_EQ(v, true);

    v = std::is_same<double, typename TypeAtNonStrict<T3, 2>::Result>::value;
    EXPECT_EQ(v, true);

    v = std::is_same<NullType, typename TypeAtNonStrict<T3, 3>::Result>::value;
    EXPECT_EQ(v, true);

    v = std::is_same<void, typename TypeAtNonStrict<T3, 4, void>::Result>::value;
    EXPECT_EQ(v, true);
}

TEST_F(TypelistTest, testIndexOf)
{
    using T3 = Typelist<int, char, double, int>;
    auto i = IndexOf<T3, int>::value;
    EXPECT_EQ(i, 0);

    i = IndexOf<T3, char>::value;
    EXPECT_EQ(i, 1);

    i = IndexOf<T3, double>::value;
    EXPECT_EQ(i, 2);

    i = IndexOf<T3, float>::value;
    EXPECT_EQ(i, -1);
}

TEST_F(TypelistTest, testAppend)
{
    using T1 = Typelist<int>;
    using T2 = typename Append<Typelist<>, int>::Result;
    auto v = std::is_same<T1, T2>::value;
    EXPECT_EQ(v, true);

    using T3 = Typelist<int, char>;
    using T4 = typename Append<Typelist<int>, char>::Result;
    v = std::is_same<T3, T4>::value;
    EXPECT_EQ(v, true);

    using T5 = Typelist<int>;
    using T6 = typename Append<Typelist<int>, Typelist<>>::Result;
    v = std::is_same<T5, T6>::value;
    EXPECT_EQ(v, true);

    using T7 = Typelist<int>;
    using T8 = typename Append<Typelist<>, Typelist<int>>::Result;
    v = std::is_same<T7, T8>::value;
    EXPECT_EQ(v, true);

    using T9 = Typelist<int, char, double, float, short>;
    using T10 = typename Append<Typelist<int, char>, Typelist<double, float, short>>::Result;
    v = std::is_same<T9, T10>::value;
    EXPECT_EQ(v, true);
}

TEST_F(TypelistTest, testErase)
{
    using T1 = Typelist<int, char, double, int, short>;
    using T2 = typename Erase<T1, int>::Result;
    auto v = std::is_same<T2, Typelist<char, double, int, short>>::value;
    EXPECT_EQ(v, true);

    using T3 = typename Erase<T1, double>::Result;
    v = std::is_same<T3, Typelist<int, char, int, short>>::value;
    EXPECT_EQ(v, true);

    using T4 = typename Erase<T1, char>::Result;
    v = std::is_same<T4, Typelist<int, double, int, short>>::value;
    EXPECT_EQ(v, true);

    using T5 = typename Erase<T1, short>::Result;
    v = std::is_same<T5, Typelist<int, char, double, int>>::value;
    EXPECT_EQ(v, true);

    using T6 = typename Erase<T1, long long>::Result;
    v = std::is_same<T6, Typelist<int, char, double, int, short>>::value;
    EXPECT_EQ(v, true);
}

TEST_F(TypelistTest, testEraseAll)
{
    using T1 = Typelist<int, char, int, int, char, double>;
    using T2 = typename EraseAll<T1, int>::Result;
    auto v = std::is_same<Typelist<char, char, double>, T2>::value;
    EXPECT_EQ(v, true);

    using T3 = typename EraseAll<T1, char>::Result;
    v = std::is_same<T3, Typelist<int, int, int, double>>::value;
    EXPECT_EQ(v, true);

    using T4 = typename EraseAll<T1, double>::Result;
    v = std::is_same<T4, Typelist<int, char, int, int, char>>::value;
    EXPECT_EQ(v, true);

    using T5 = typename EraseAll<T1, short>::Result;
    v = std::is_same<T5, T1>::value;
    EXPECT_EQ(v, true);
}

TEST_F(TypelistTest, testNoDuplicates)
{
    using T1 = Typelist<>;
    using T2 = typename NoDuplicates<T1>::Result;
    auto v = std::is_same<Typelist<>, T2>::value;
    EXPECT_EQ(v, true);

    using T3 = Typelist<int, char>;
    using T4 = typename NoDuplicates<T3>::Result;
    v = std::is_same<Typelist<int, char>, T4>::value;
    EXPECT_EQ(v, true);

    using T5 = Typelist<int, char, int, int>;
    using T6 = typename NoDuplicates<T3>::Result;
    v = std::is_same<Typelist<int, char>, T6>::value;
    EXPECT_EQ(v, true);

    using T7 = Typelist<int, char, char, int, double, float, char, int>;
    using T8 = typename NoDuplicates<T7>::Result;
    v = std::is_same<Typelist<int, char, double, float>, T8>::value;
    EXPECT_EQ(v, true);
}

TEST_F(TypelistTest, testReplace)
{
    using T1 = Typelist<>;
    using T2 = typename Replace<T1, int, char>::Result;
    auto v = std::is_same<T2, Typelist<>>::value;
    EXPECT_EQ(v, true);

    using T3 = Typelist<int, char, int, float>;
    using T4 = typename Replace<T3, int, short>::Result;
    v = std::is_same<T4, Typelist<short, char, int, float>>::value;
    EXPECT_EQ(v, true);

    using T6 = typename Replace<T3, float, short>::Result;
    v = std::is_same<T6, Typelist<int, char, int, short>>::value;
    EXPECT_EQ(v, true);

    using T8 = typename Replace<T3, void, short>::Result;
    v = std::is_same<T8, Typelist<int, char, int, float>>::value;
    EXPECT_EQ(v, true);
}

TEST_F(TypelistTest, testReplaceAll)
{
    using T1 = Typelist<>;
    using T2 = typename ReplaceAll<T1, int, char>::Result;
    auto v = std::is_same<T2, Typelist<>>::value;
    EXPECT_EQ(v, true);

    using T3 = Typelist<int, char, int, float>;
    using T4 = typename ReplaceAll<T3, int, short>::Result;
    v = std::is_same<T4, Typelist<short, char, short, float>>::value;
    EXPECT_EQ(v, true);

    using T6 = typename ReplaceAll<T3, float, short>::Result;
    v = std::is_same<T6, Typelist<int, char, int, short>>::value;
    EXPECT_EQ(v, true);

    using T8 = typename ReplaceAll<T3, void, short>::Result;
    v = std::is_same<T8, Typelist<int, char, int, float>>::value;
    EXPECT_EQ(v, true);
}

TEST_F(TypelistTest, testReverse)
{
    using T1 = typename Reverse<Typelist<>>::Result;
    auto v = std::is_same<T1, Typelist<>>::value;
    EXPECT_EQ(v, true);

    using T2 = typename Reverse<Typelist<int, double, char>>::Result;
    v = std::is_same<T2, Typelist<char, double, int>>::value;
    EXPECT_EQ(v, true);

    using T3 = typename Reverse<Typelist<int, double, int, char>>::Result;
    v = std::is_same<T3, Typelist<char, int, double, int>>::value;
    EXPECT_EQ(v, true);

    using T4 = typename Reverse<Typelist<int>>::Result;
    v = std::is_same<T4, Typelist<int>>::value;
    EXPECT_EQ(v, true);
}

class A{};
class B : public A{};
class C : public B{};
class D : public A{};

TEST_F(TypelistTest, testMostDerived)
{
    using T1 = Typelist<int, char>;
    auto v = std::is_same<typename MostDerived<T1, float>::Result, float>::value;
    EXPECT_EQ(v, true);

    using T2 = Typelist<A, B>;
    v = std::is_same<typename MostDerived<T2, C>::Result, C>::value;
    EXPECT_EQ(v, true);

    using T3 = Typelist<A, B, C>;
    v = std::is_same<typename MostDerived<T3, D>::Result, D>::value;
    EXPECT_EQ(v, true);

    using T4 = Typelist<A, C, B, D>;
    v = std::is_same<typename MostDerived<T4, int>::Result, int>::value;
    EXPECT_EQ(v, true);

    using T5 = Typelist<A, C, B, D>;
    v = std::is_same<typename MostDerived<T5, A>::Result, C>::value;
    EXPECT_EQ(v, true);

    using T6 = Typelist<A, D, C, B>;
    v = std::is_same<typename MostDerived<T6, A>::Result, D>::value;
    EXPECT_EQ(v, true);

    using T7 = Typelist<A, D>;
    v = std::is_same<typename MostDerived<T7, B>::Result, B>::value;
    EXPECT_EQ(v, true);
}

TEST_F(TypelistTest, testDerivedToFront)
{
    using T1 = Typelist<>;
    auto v = std::is_same<typename DerivedToFront<T1>::Result,
                          Typelist<>
                          >::value;
    EXPECT_EQ(v, true);

    using T2 = Typelist<A>;
    v = std::is_same<typename DerivedToFront<T2>::Result,
                     Typelist<A>
                     >::value;
    EXPECT_EQ(v, true);

    using T3 = Typelist<int, char, A>;
    v = std::is_same<typename DerivedToFront<T3>::Result,
                     Typelist<int, char, A>
                     >::value;
    EXPECT_EQ(v, true);

    using T4 = Typelist<int, char, A, D>;
    v = std::is_same<typename DerivedToFront<T4>::Result,
                     Typelist<int, char, D, A>
                     >::value;
    EXPECT_EQ(v, true);

    using T5 = Typelist<A, D, B, C>;
    v = std::is_same<typename DerivedToFront<T5>::Result,
                     Typelist<D, C, B, A>
                     >::value;
    EXPECT_EQ(v, true);

    using T6 = Typelist<A, B, D, C>;
    v = std::is_same<typename DerivedToFront<T6>::Result,
                     Typelist<C, B, D, A>
                     >::value;
    EXPECT_EQ(v, true);

    using T7 = Typelist<A, D, C>;
    v = std::is_same<typename DerivedToFront<T7>::Result,
                     Typelist<D, C, A>
                     >::value;
    EXPECT_EQ(v, true);
}
