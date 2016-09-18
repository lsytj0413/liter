#include "aspectTest.h"

#include <gtest/gtest.h>
#include <liter/aspect.hpp>
#include "aspectTest.h"


class TestHasMemberImp{
public:
    void imp_member1(){};
    void imp_member4(int){};
    void imp_member5(double, char, int){};

    virtual void vir_member(int){};

    static void static_member(int){};

protected:
    void imp_member2(){};

private:
    void imp_member3(){};
};

HAS_MEMBER(imp_member1);
HAS_MEMBER(imp_member2);
HAS_MEMBER(imp_member3);
HAS_MEMBER(imp_member4);
HAS_MEMBER(imp_member5);


TEST_F(AspectTest, testHasMemberOwn)
{
    auto v0 = has_member_imp_member1<TestHasMemberImp>::value;
    EXPECT_EQ(v0, true);

    // have args: false
    auto v01 = has_member_imp_member1<TestHasMemberImp, void>::value;
    EXPECT_EQ(v01, false);

    auto v02 = has_member_imp_member1<TestHasMemberImp, int>::value;
    EXPECT_EQ(v02, false);

    // protected false
    auto v11 = has_member_imp_member2<TestHasMemberImp>::value;
    EXPECT_EQ(v11, false);

    // private false
    auto v2 = has_member_imp_member3<TestHasMemberImp>::value;
    EXPECT_EQ(v2, false);

    auto v30 = has_member_imp_member4<TestHasMemberImp, int>::value;
    EXPECT_EQ(v30, true);

    // 能够隐式转换， true
    auto v31 = has_member_imp_member4<TestHasMemberImp, char>::value;
    EXPECT_EQ(v31, true);

    // wrong args: false
    auto v34 = has_member_imp_member4<TestHasMemberImp, double>::value;
    EXPECT_EQ(v34, true);

    auto v32 = has_member_imp_member4<TestHasMemberImp>::value;
    EXPECT_EQ(v32, false);

    auto v33 = has_member_imp_member4<TestHasMemberImp, int, double>::value;
    EXPECT_EQ(v33, false);

    // 应该是false的，不知为啥
    auto v40 = has_member_imp_member5<TestHasMemberImp, char, double, int>::value;
    EXPECT_EQ(v40, true);

    auto v41 = has_member_imp_member5<TestHasMemberImp, double, char, int>::value;
    EXPECT_EQ(v41, true);
}


HAS_MEMBER(vir_member);

TEST_F(AspectTest, testHasMemberVirtual)
{
    auto v0 = has_member_vir_member<TestHasMemberImp, int>::value;
    EXPECT_EQ(v0, true);

    auto v1 = has_member_vir_member<TestHasMemberImp>::value;
    EXPECT_EQ(v1, false);

    auto v2 = has_member_vir_member<TestHasMemberImp, int, char>::value;
    EXPECT_EQ(v2, false);
}

HAS_MEMBER(static_member);

TEST_F(AspectTest, testHasMemberStatic)
{
    auto v0 = has_member_static_member<TestHasMemberImp, int>::value;
    EXPECT_EQ(v0, true);

    auto v1 = has_member_static_member<TestHasMemberImp>::value;
    EXPECT_EQ(v1, false);

    auto v2 = has_member_static_member<TestHasMemberImp, int, char>::value;
    EXPECT_EQ(v2, false);
}

class TestHasMember : public TestHasMemberImp
{
public:
    void member1(int){};
};

HAS_MEMBER(member1);


TEST_F(AspectTest, testHasMemberChild)
{
    auto v0 = has_member_member1<TestHasMember, int>::value;
    EXPECT_EQ(v0, true);

    auto v1 = has_member_member1<TestHasMember>::value;
    EXPECT_EQ(v1, false);

    auto v2 = has_member_member1<TestHasMember, char, int>::value;
    EXPECT_EQ(v2, false);
}


TEST_F(AspectTest, testHasMemberInhe)
{
    auto v0 = has_member_imp_member1<TestHasMember>::value;
    EXPECT_EQ(v0, true);

    // have args: false
    auto v01 = has_member_imp_member1<TestHasMember, void>::value;
    EXPECT_EQ(v01, false);

    auto v02 = has_member_imp_member1<TestHasMember, int>::value;
    EXPECT_EQ(v02, false);

    // protected false
    auto v11 = has_member_imp_member2<TestHasMember>::value;
    EXPECT_EQ(v11, false);

    // private false
    auto v2 = has_member_imp_member3<TestHasMember>::value;
    EXPECT_EQ(v2, false);
}


TEST_F(AspectTest, TestHasMemberInheVir)
{
    auto v0 = has_member_vir_member<TestHasMember, int>::value;
    EXPECT_EQ(v0, true);

    auto v1 = has_member_vir_member<TestHasMember>::value;
    EXPECT_EQ(v1, false);

    auto v2 = has_member_vir_member<TestHasMember, int, char>::value;
    EXPECT_EQ(v2, false);
}


int g = 0;

class A
{
public:
    void before(int i){
        g += i;
    };
};

class B
{
public:
    void after(int i){
        g -= i;
    };
};

class C
{
public:
    void before(int i){
        g += i;
    };

    void after(int i){
        g -= i;
    };
};

void test(int i){};

TEST_F(AspectTest, testOneInvokeBefore)
{
    liter::invoke<A>(test, 100);
    EXPECT_EQ(100, g);

    g = 0;
}


TEST_F(AspectTest, testOneInvokeAfter)
{
    liter::invoke<B>(test, 10);
    EXPECT_EQ(-10, g);

    g = 0;
}


TEST_F(AspectTest, testOneInvoke)
{
    g = 10;

    liter::invoke<C>(test, 100);
    EXPECT_EQ(g, 10);

    g = 0;
}
