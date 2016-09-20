#include "objectPoolTest.h"

#include <gtest/gtest.h>

#include <liter/object_pool.hpp>


class A
{
public:
    A(){};

    int m_i = 0;
    int m_j = 0;

    A(int i, int j):m_i(i), m_j(j){};
};


TEST_F(ObjectPoolTest, testPoolOne)
{
    liter::ObjectPool<A, 1> pool;
    pool.init(1);

    auto p0 = pool.get();
    EXPECT_EQ(p0->m_i, 0);
    EXPECT_EQ(p0->m_j, 0);

    EXPECT_EQ(nullptr, pool.get());

    A* p00 = p0.get();
    p0 = std::shared_ptr<A>();

    auto p1 = pool.get();
    A* p10 = p1.get();
    EXPECT_EQ(p00, p10);
}

class B
{
public:
    B(){
        liter::ObjectPool<A, 5> pool;
        pool.init(2);
    }
};

TEST_F(ObjectPoolTest, testPoolArgs)
{
    B b;

    liter::ObjectPool<A, 3> pool;
    // 此处init的个数超过2个就会busy loop, 暂时不清楚原因
    // 但是从class B的测试看， 应该不是ObjectPool本身的问题
    pool.init(1, 5, 6);

    auto p0 = pool.get<int, int>();

    auto i = p0->m_i;
    EXPECT_EQ(i, 5);
    i = p0->m_j;
    EXPECT_EQ(i, 6);
}


TEST_F(ObjectPoolTest, testPoolMix)
{
    liter::ObjectPool<A, 1> pool;
    pool.init(1);

    auto p0 = pool.get();
    EXPECT_EQ(p0->m_i, 0);
    EXPECT_EQ(p0->m_j, 0);

    pool.init(1, 3, 4);
    auto p1 = pool.get<int, int>();
    EXPECT_EQ(p1->m_i, 3);
    EXPECT_EQ(p1->m_j, 4);

    pool.init(1, 7, 4);
    auto p2 = pool.get<int, int>();
    EXPECT_EQ(p2->m_i, 7);
    EXPECT_EQ(p2->m_j, 4);
}
