#include "callableTest.h"

#include <gtest/gtest.h>
#include <liter/callable.h>


int global_var = 0;

void assign_none(){
    global_var = 100;
};

void assign_one(int v){
    global_var = v;
};

void assign_multi(int v1, int v2){
    global_var = v1 + v2;
};

void assign_multi(int v1, int v2, int v3){
    global_var = v1 + v2 + v3;
};


TEST_F(CallableTest, testAssignNone)
{
    liter::functor_call(assign_none);
    EXPECT_EQ(global_var, 100);
}


TEST_F(CallableTest, testAssignOne)
{
    liter::functor_call(assign_one, 3);
    EXPECT_EQ(global_var, 3);

    liter::functor_call(assign_one, 4.5);
    EXPECT_EQ(global_var, 4);
}


TEST_F(CallableTest, testAssignMulti)
{
    using f1 = void(*)(int, int);
    liter::functor_call(static_cast<f1>(assign_multi), 1, 2);
    EXPECT_EQ(global_var, 3);

    liter::functor_call(static_cast<f1>(assign_multi), 100, 300.5);
    EXPECT_EQ(global_var, 400);

    using f2 = void(*)(int, int, int);
    liter::functor_call(static_cast<f2>(assign_multi), 1, 3, 5);
    EXPECT_EQ(global_var, 9);

    liter::functor_call(static_cast<f2>(assign_multi), 1, 30, 5.5);
    EXPECT_EQ(global_var, 36);
}
