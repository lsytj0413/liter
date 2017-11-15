#include "podReflectionTest.h"

#include <gtest/gtest.h>
#include <liter/pod_reflection.hpp>

#include <cstdio>
#include <typeindex>
#include <memory>
// #include <boost/type_index.hpp>

using namespace liter;

struct foo {
    unsigned char i0;
    unsigned int i1;
    unsigned short i2;
    unsigned long long i3;
    unsigned char ar[2];
    int q;
    std::size_t w;
    int* p1;
    const void* p2;
    int const**const**** p_crazy;
    const double d;
};

template <std::size_t I, class T>
void print(T& f) {
    // printf("%lu\t\t%s\n",
    //        (std::size_t)get<I>(f),
    //        boost::typeindex::type_id<decltype(get<I>(f))>().pretty_name().c_str());

    printf("%lu\t\t\n",
           (std::size_t)get<I>(f));
}

TEST_F(PodReflectionTest, testAll)
{
    foo f {10, 11, 12, 13, {14, 15}, 16, 17, 0, 0, 0, 30.0};
    // print<0>(f);  print<1>(f);  print<2>(f);
    // print<3>(f);  print<4>(f);  print<5>(f);
    // print<6>(f);  print<7>(f);  print<8>(f);
    // print<9>(f);  print<10>(f); print<11>(f);
    // static_assert(tuple_size<foo>() == 12, "failed tuple size check");
    EXPECT_EQ(12, tuple_size<foo>());

    EXPECT_EQ(10, get<0>(f));
    EXPECT_EQ(11, get<1>(f));
    EXPECT_EQ(12, get<2>(f));
    EXPECT_EQ(13, get<3>(f));
    EXPECT_EQ(14, get<4>(f));
    EXPECT_EQ(15, get<5>(f));
    EXPECT_EQ(16, get<6>(f));
    EXPECT_EQ(17, get<7>(f));
    EXPECT_EQ(0, get<8>(f));
    EXPECT_EQ(0, get<9>(f));
    EXPECT_EQ(0, get<10>(f));
    EXPECT_FLOAT_EQ(30, get<11>(f));

    // std::cout<<"finish"<<std::endl;
}
