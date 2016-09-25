#include "visitorTest.h"

#include <gtest/gtest.h>

#include <liter/visitor.hpp>

static int g = 0;

class A;
class B;

using VisitorAB = liter::visitor<A, B>;

class A
{
public:
    void visit(VisitorAB& v){
        v.visit(*this);
    }

    int i = 10;
};

class B
{
public:
    void visit(VisitorAB& v){
        v.visit(*this);
    };

    int i = 15;
};

class CVAB : public VisitorAB
{
public:
    virtual void visit(const A& a){
        g += a.i;
    };

    virtual void visit(const B& b){
        g += b.i;
    };
};


TEST_F(VisitorTest, testVisit)
{
    CVAB v;
    A aobj;
    B bobj;

    g = 0;

    aobj.visit(v);
    EXPECT_EQ(g, 10);

    bobj.visit(v);
    EXPECT_EQ(g, 25);

    g = 0;

    bobj.visit(v);
    EXPECT_EQ(g, 15);
}
