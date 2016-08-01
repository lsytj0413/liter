#include "stringUtilTest.h"

#include <gtest/gtest.h>
#include <liter/utils/string_utils.h>


TEST_F(StringUtilTest, testWhitespacepChar)
{
    EXPECT_EQ(true, liter::whitespace_p(' '));
    EXPECT_EQ(true, liter::whitespace_p('\t'));
    EXPECT_EQ(false, liter::whitespace_p('1'));
    EXPECT_EQ(false, liter::whitespace_p('a'));
    EXPECT_EQ(false, liter::whitespace_p('B'));
}

TEST_F(StringUtilTest, testWhitespacepString)
{
    EXPECT_EQ(true, liter::whitespace_p(""));
    EXPECT_EQ(true, liter::whitespace_p("    "));
    EXPECT_EQ(false, liter::whitespace_p("  a"));
    EXPECT_EQ(false, liter::whitespace_p("a  b"));
    EXPECT_EQ(false, liter::whitespace_p("db  c  "));
}
