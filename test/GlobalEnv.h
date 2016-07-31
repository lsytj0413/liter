#pragma once

#include <iostream>
#include <gtest/gtest.h>

class GlobalEnv: public testing::Environment
{
public:
    virtual void SetUp()
    {
        std::cout << "Env SetUp" << std::endl;
    }

    virtual void TearDown()
    {
        std::cout << "Env TearDown" << std::endl;
    }
};
