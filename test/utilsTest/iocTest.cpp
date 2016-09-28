#include "iocTest.h"

#include <gtest/gtest.h>

#include <liter/ioc.hpp>

static int g = 0;

class A0{
public:
    void f0(){
        g = 10;
    };

    void f1(int i){
        g += i;
    };

    A0(){};

    int m_i;
    A0(int i) : m_i(i){};

    int g0(){return m_i;};
};

class B0{
public:
    A0* m_p;
    B0(A0* p) : m_p(p){};
};

TEST_F(IocTest, testUndependType)
{
    g = 0;

    liter::Ioc obj;
    obj.register_type<B0, A0>("B0");

    EXPECT_EQ(g, 0);
    auto p = obj.resolve<B0>("B0");
    p->m_p->f0();

    EXPECT_EQ(g, 10);
    p->m_p->f1(20);
    EXPECT_EQ(g, 30);

    obj.register_type<B0, A0, int>("B0Z");
    p = obj.resolve<B0>("B0Z", 5);
    EXPECT_EQ(p->m_p->g0(), 5);
}

class A1{
public:
    virtual int f0(){
        return 10;
    };

    virtual int f1(){
        return 30;
    };

    A1(){};
    int m_i;
    A1(int i, int j) : m_i(i + j){};
};

class B1 : public A1 {
public:
    virtual int f0(){
        return 20;
    };

    using A1::A1;
};

TEST_F(IocTest, testDependType)
{
    g = 0;

    liter::Ioc obj;
    obj.register_type<A1, B1>("B1");

    auto p = obj.resolve_shared<A1>("B1");
    EXPECT_EQ(p->f0(), 20);
    EXPECT_EQ(p->f1(), 30);

    auto v = std::is_same<decltype(p), std::shared_ptr<A1>>::value;
    EXPECT_EQ(true, v);

    obj.register_type<A1, B1, int, int>("B1Z");
    p = obj.resolve_shared<A1>("B1Z", 10, 50);

    EXPECT_EQ(p->m_i, 60);
}
