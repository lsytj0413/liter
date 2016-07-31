#pragma once

#include <iostream>
#include <gtest/gtest.h>

class BinaryTest : public testing::Test
{
protected:
    static void SetUpTestCase()
    {
        std::cout << "BinaryTest SetUpTestCase" << std::endl;
    }

    static void TearDownTestCase()
    {
        std::cout << "BinaryTest TearDownTestCase" << std::endl;
    }

    virtual void SetUp()
    {
        std::cout << "BinaryTest SetUp" << std::endl;
    }

    virtual void TearDown()
    {
        std::cout << "BinaryTest TearDown" << std::endl;
    }
};
