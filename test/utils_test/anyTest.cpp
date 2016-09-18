#include "anyTest.h"


#include <gtest/gtest.h>
#include <liter/any.hpp>

#include <string>


TEST_F(AnyTest, testNull)
{
    auto v = liter::any();
    EXPECT_EQ(true, v.null_p());

    v = liter::any(1);
    EXPECT_EQ(false, v.null_p());
}


TEST_F(AnyTest, testIs)
{
    auto v = liter::any();
    EXPECT_EQ(false, v.is<int>());
    EXPECT_EQ(true, v.is<void>());

    v = liter::any(int(1));
    EXPECT_EQ(false, v.is<void>());
    EXPECT_EQ(true, v.is<int>());

    v = liter::any(double(1.1));
    EXPECT_EQ(false, v.is<int>());
    EXPECT_EQ(true, v.is<double>());

    v = liter::any(std::string("any"));
    EXPECT_EQ(false, v.is<double>());
    EXPECT_EQ(true, v.is<std::string>());
}


TEST_F(AnyTest, testNonullCast)
{
    auto v = liter::any(int(1));
    bool bad = false;

    try{
        auto i = v.cast<int>();
        EXPECT_EQ(i, 1);
    }
    catch(std::bad_cast& e){
        bad = true;
    }
    EXPECT_EQ(false, bad);

    {
        try{
            auto i = v.cast<double>();
            bad = false;
        }
        catch(std::logic_error& e){
            bad = true;
        }
    }
    EXPECT_EQ(true, bad);

    v = liter::any(double(1.1));
    {
        try{
            auto i = v.cast<double>();
            EXPECT_FLOAT_EQ(i, 1.1);
            bad = false;
        }
        catch(std::logic_error& e){
            bad = true;
        }
    }
    EXPECT_EQ(false, bad);

    v = liter::any(std::string("test"));
    try{
        auto i = v.cast<std::string>();
        EXPECT_EQ(i, std::string("test"));
        bad = false;
    }
    catch(std::logic_error& e){
        bad = true;
    }
    EXPECT_EQ(false, bad);

    try{
        auto i = v.cast<char*>();
        bad = false;
    }
    catch(std::logic_error& e){
        bad = true;
    }
    EXPECT_EQ(true, bad);
}


TEST_F(AnyTest, testNullCast)
{
    auto v = liter::any();
    auto bad = false;

    try{
        auto i = v.cast<char*>();
        bad = false;
    }
    catch(std::logic_error& e){
        bad = true;
    }
    EXPECT_EQ(true, bad);
}
