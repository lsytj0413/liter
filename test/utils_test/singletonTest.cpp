#include "singletonTest.h"

#include <liter/utils/Singleton.h>

class S : public liter::Singleton<S>
{
    friend class liter::Singleton<S>;

private:
    S() : i(0){};
    int i;

public:
    void add()
    {
        ++i;
    }

    int get()
    {
        return i;
    }
};

TEST_F(SingletonTest, testSingle)
{
    S* p1 = S::Instance();
    EXPECT_EQ(0, p1->get());

    p1->add();
    EXPECT_EQ(1, p1->get());

    S* p2 = S::Instance();
    EXPECT_EQ(1, p2->get());

    p2->add();
    EXPECT_EQ(2, p2->get());

    EXPECT_EQ(2, p2->get());

    EXPECT_EQ(p1, p2);
}
