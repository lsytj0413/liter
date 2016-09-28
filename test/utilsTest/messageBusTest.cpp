#include "messageBusTest.h"

#include <gtest/gtest.h>

#include <liter/message_bus.hpp>
#include <functional>


static int g = 0;

static void f0(){
    g += 10;
};

class A{
public:
    void f0(){
        g += 20;
    };
};

TEST_F(MessageBusTest, testArgsZero)
{
    g = 0;

    liter::MessageBus m;

    m.attach(f0);
    EXPECT_EQ(g, 0);

    m.send<void>();
    EXPECT_EQ(g, 10);

    g = 0;
    m.remove<void>();
    EXPECT_EQ(g, 0);
    m.send<void>();
    EXPECT_EQ(g, 0);

    auto p = new A();
    std::function<void()> fn = std::bind(&A::f0, p);
    m.attach(fn);

    m.send<void>();
    EXPECT_EQ(g, 20);

    g = 0;
    m.attach(f0);

    m.send<void>();
    EXPECT_EQ(g, 30);
}

static void f2(int i, int j){
    g += i;
    g += j;
};

class C{
public:
    void f0(int i, int j, int k){
        g += i;
        g += j;
        g += k;
    };
};


TEST_F(MessageBusTest, testArgsMulti)
{
    g = 0;

    liter::MessageBus m;

    m.attach(f2);
    EXPECT_EQ(g, 0);

    m.send<void, int, int>(10, 5);
    EXPECT_EQ(g, 15);

    g = 0;
    m.remove<void, int, int>();
    EXPECT_EQ(g, 0);
    m.send<void, int, int>(10, 5);
    EXPECT_EQ(g, 0);

    auto p = new C();
    std::function<void(int, int)> fn = std::bind(&C::f0, p, std::placeholders::_1, std::placeholders::_2, 30);
    m.attach(fn);

    m.send<void, int, int>(5, 5);
    EXPECT_EQ(g, 40);

    g = 0;
    m.attach(f2);

    m.send<void, int, int>(5, 5);
    EXPECT_EQ(g, 50);
}
