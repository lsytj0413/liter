#include "eventsTest.h"

#include <gtest/gtest.h>

#include <liter/events.hpp>

#include <functional>
using std::function;
using std::bind;


static int g = 0;

static void test0(){
    g = 10;
};

static void test1(int i){
    g += i;
};

static void test2(int i, int j){
    g += i;
    g += j;
};


TEST_F(EventsTest, testArgsZero)
{
    g = 0;

    using E = liter::events<std::function<void()>>;
    E eobj;

    EXPECT_EQ(g, 0);

    auto id = eobj.connect(test0);
    EXPECT_EQ(g, 0);

    eobj.notify();
    EXPECT_EQ(g, 10);

    g = 0;
    EXPECT_EQ(g, 0);
    eobj.disconnect(id);
    EXPECT_EQ(g, 0);

    eobj.notify();
    EXPECT_EQ(g, 0);
}


TEST_F(EventsTest, testArgsOne)
{
    g = 0;

    liter::events<std::function<void(int)>> eobj;
    auto id1 = eobj.connect(test1);

    EXPECT_EQ(g, 0);
    eobj.notify(10);
    EXPECT_EQ(g, 10);

    auto id2 = eobj.connect(test1);

    eobj.notify(20);
    EXPECT_EQ(g, 50);

    eobj.disconnect(id1);
    eobj.notify(10);
    EXPECT_EQ(g, 60);

    eobj.disconnect(id2);
    eobj.notify(10);
    EXPECT_EQ(g, 60);
}


TEST_F(EventsTest, testArgsMulti)
{
    g = 0;

    liter::events<std::function<void(int, int)>> eobj;
    auto id = eobj.connect(test2);

    EXPECT_EQ(g, 0);
    eobj.notify(10, 40);
    EXPECT_EQ(g, 50);
}
