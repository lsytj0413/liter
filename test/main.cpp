#include <gtest/gtest.h>

#include "GlobalEnv.h"


int main(int argc, char* argv[])
{
    testing::AddGlobalTestEnvironment(new GlobalEnv());
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
