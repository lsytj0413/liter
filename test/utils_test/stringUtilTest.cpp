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

TEST_F(StringUtilTest, testIntp)
{
    EXPECT_EQ(true, liter::int_p("123"));
    EXPECT_EQ(true, liter::int_p("00223"));
    EXPECT_EQ(true, liter::int_p("320043"));
    EXPECT_EQ(true, liter::int_p("3200"));
    EXPECT_EQ(true, liter::int_p("00"));

    EXPECT_EQ(false, liter::int_p(""));
    EXPECT_EQ(false, liter::int_p("ab"));
    EXPECT_EQ(false, liter::int_p("02c"));
    EXPECT_EQ(false, liter::int_p("3234."));
    EXPECT_EQ(false, liter::int_p(".234234"));
    EXPECT_EQ(false, liter::int_p("sfs.2252"));
    EXPECT_EQ(false, liter::int_p("232.3423"));
    EXPECT_EQ(false, liter::int_p(" sdfs "));
}
