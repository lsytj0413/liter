#include "stringUtilTest.h"

#include <gtest/gtest.h>
#include <liter/string_utils.h>

#include <string>
using std::string;

using namespace liter;

TEST_F(StringUtilTest, testWhitespacepChar)
{
    EXPECT_EQ(true, StringUtils::whitespace_p(' '));
    EXPECT_EQ(true, StringUtils::whitespace_p('\t'));
    EXPECT_EQ(false, StringUtils::whitespace_p('1'));
    EXPECT_EQ(false, StringUtils::whitespace_p('a'));
    EXPECT_EQ(false, StringUtils::whitespace_p('B'));
}

TEST_F(StringUtilTest, testWhitespacepString)
{
    EXPECT_EQ(true, StringUtils::whitespace_p(""));
    EXPECT_EQ(true, StringUtils::whitespace_p("    "));
    EXPECT_EQ(false, StringUtils::whitespace_p("  a"));
    EXPECT_EQ(false, StringUtils::whitespace_p("a  b"));
    EXPECT_EQ(false, StringUtils::whitespace_p("db  c  "));
}

TEST_F(StringUtilTest, testIntp)
{
    EXPECT_EQ(true, StringUtils::int_p("123"));
    EXPECT_EQ(true, StringUtils::int_p("00223"));
    EXPECT_EQ(true, StringUtils::int_p("320043"));
    EXPECT_EQ(true, StringUtils::int_p("3200"));
    EXPECT_EQ(true, StringUtils::int_p("00"));

    EXPECT_EQ(false, StringUtils::int_p(""));
    EXPECT_EQ(false, StringUtils::int_p("ab"));
    EXPECT_EQ(false, StringUtils::int_p("02c"));
    EXPECT_EQ(false, StringUtils::int_p("3234."));
    EXPECT_EQ(false, StringUtils::int_p(".234234"));
    EXPECT_EQ(false, StringUtils::int_p("sfs.2252"));
    EXPECT_EQ(false, StringUtils::int_p("232.3423"));
    EXPECT_EQ(false, StringUtils::int_p(" sdfs "));
}

TEST_F(StringUtilTest, testFloatp)
{
    EXPECT_EQ(true, StringUtils::float_p("123"));
    EXPECT_EQ(true, StringUtils::float_p("00223"));
    EXPECT_EQ(true, StringUtils::float_p("320043"));
    EXPECT_EQ(true, StringUtils::float_p("3200"));
    EXPECT_EQ(true, StringUtils::float_p("00"));
    EXPECT_EQ(true, StringUtils::float_p("00.0"));
    EXPECT_EQ(true, StringUtils::float_p("1.00"));
    EXPECT_EQ(true, StringUtils::float_p("0.0"));
    EXPECT_EQ(true, StringUtils::float_p("232.3423"));

    EXPECT_EQ(false, StringUtils::float_p(""));
    EXPECT_EQ(false, StringUtils::float_p("ab"));
    EXPECT_EQ(false, StringUtils::float_p("02c"));
    EXPECT_EQ(false, StringUtils::float_p("3234."));
    EXPECT_EQ(false, StringUtils::float_p(".234234"));
    EXPECT_EQ(false, StringUtils::float_p("sfs.2252"));
    EXPECT_EQ(false, StringUtils::float_p(" sdfs "));
    EXPECT_EQ(false, StringUtils::float_p(".0"));
    EXPECT_EQ(false, StringUtils::float_p("0."));
    EXPECT_EQ(false, StringUtils::float_p("0.0.0"));
}


TEST_F(StringUtilTest, testTrimLeft)
{
    string s1 = "  abd";
    auto v1 = StringUtils::trim_left(s1);
    EXPECT_EQ(v1, string("abd"));

    string s2 = "a  bd";
    auto v2 = StringUtils::trim_left(s2);
    EXPECT_EQ(v2, string("a  bd"));

    string s3 = "  a  bd";
    auto v3 = StringUtils::trim_left(s3);
    EXPECT_EQ(v3, string("a  bd"));

    string s4 = "\tabd";
    auto v4 = StringUtils::trim_left(s4);
    EXPECT_EQ(v4, string("abd"));

    string s5 = "\t abd \t";
    auto v5 = StringUtils::trim_left(s5);
    EXPECT_EQ(v5, string("abd \t"));
}

TEST_F(StringUtilTest, testTrimRight)
{
    string s1 = "abd  ";
    auto v1 = StringUtils::trim_right(s1);
    EXPECT_EQ(v1, string("abd"));

    string s2 = "a  bd";
    auto v2 = StringUtils::trim_right(s2);
    EXPECT_EQ(v2, string("a  bd"));

    string s3 = "a  bd  ";
    auto v3 = StringUtils::trim_right(s3);
    EXPECT_EQ(v3, string("a  bd"));

    string s4 = "abd\t";
    auto v4 = StringUtils::trim_right(s4);
    EXPECT_EQ(v4, string("abd"));

    string s5 = "\t abd \t";
    auto v5 = StringUtils::trim_right(s5);
    EXPECT_EQ(v5, string("\t abd"));
}

TEST_F(StringUtilTest, testTrim)
{
    string s1 = "\tabd  ";
    auto v1 = StringUtils::trim(s1);
    EXPECT_EQ(v1, string("abd"));

    string s2 = "a  bd";
    auto v2 = StringUtils::trim(s2);
    EXPECT_EQ(v2, string("a  bd"));

    string s3 = "  a  bd  ";
    auto v3 = StringUtils::trim(s3);
    EXPECT_EQ(v3, string("a  bd"));

    string s4 = "  abd\t";
    auto v4 = StringUtils::trim(s4);
    EXPECT_EQ(v4, string("abd"));

    string s5 = "\t abd \t";
    auto v5 = StringUtils::trim(s5);
    EXPECT_EQ(v5, string("abd"));

    string s6 = "";
    auto v6 = StringUtils::trim(s6);
    EXPECT_EQ(true, v6.empty());

    string s7 = "  ";
    auto v7 = StringUtils::trim(s7);
    EXPECT_EQ(true, v7.empty());
}
