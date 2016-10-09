// @file string_utils.cpp
// @brief 一些字符串处理相关函数
// @author
// @version
// @date

#pragma once

#include <string>
#include <cctype>
#include <functional>
#include <algorithm>
using std::string;
using std::function;


namespace liter
{

// @struct StringUtils
// @brief 字符串工具类
struct StringUtils
{
    // @function
    // @brief 判断字符是否是数字
    // @param val_ch: 字符
    // @return bool
    static bool numeric_p(char val_ch)
    {
        return isdigit(val_ch);
    }

    // @function
    // @brief 判断字符是否是空白
    // @param val_ch: 字符
    // @return bool
    static bool whitespace_p(char val_ch)
    {
        return ' ' == val_ch ||
                '\t' == val_ch;
    }

    // @function
    // @brief 判断字符串是否是空白
    // @param val_str: 字符串
    // @return bool
    static bool whitespace_p(const string& val_str)
    {
        for (auto&& val_ch : val_str)
        {
            if (!whitespace_p(val_ch))
            {
                return false;
            }
        }

        return true;
    }

    // @function
    // @brief 判断字符串是否是整形数字
    // @param val_str: 字符串
    // @return bool
    static bool int_p(const string& val_str)
    {
        using StatFunc = std::function<bool(char)>;

        int status = 0;
        std::array<StatFunc, 3> stat_funcs;
        stat_funcs[0] = {[&](char val_ch) -> bool{
                // 处理初始状态
                if ('-' == val_ch)
                {
                    status = 1;
                }
                else if (numeric_p(val_ch))
                {
                    status = 2;
                }
                else
                {
                    return false;
                }

                return true;
            } };
        stat_funcs[1] = {[&](char val_ch) -> bool {
                // 处理有负号的状态
                if (!numeric_p(val_ch))
                {
                    return false;
                }

                status = 2;
                return true;
            } };
        stat_funcs[2] = {[&](char val_ch) -> bool {
                // 处理数字序列状态
                if (!numeric_p(val_ch))
                {
                    return false;
                }
                return true;
            } };

        for (auto&& val_ch : val_str)
        {
            if (!stat_funcs[status](val_ch))
            {
                return false;
            }
        }

        return 2 == status;
    }

    // @function
    // @brief 判断字符串是否是浮点型数字
    // @param val_str: 字符串
    // @return bool
    static bool float_p(const string& val_str)
    {
        struct SStatFunc
        {
            std::function<bool(char, int&)> stat_func;
        };
        using StatFunc = std::function<bool(char)>;

        int status = 0;
        std::array<StatFunc, 5> stat_funcs;
        stat_funcs[0] = { [&](char val_ch) -> bool {
                if ('-' == val_ch)
                {
                    status = 1;
                }
                else if (numeric_p(val_ch))
                {
                    status = 2;
                }
                else
                {
                    return false;
                }

                return true;
            } };
        stat_funcs[1] = { [&](char val_ch) -> bool {
                if (!numeric_p(val_ch))
                {
                    return false;
                }

                status = 2;
                return true;
            } };
        stat_funcs[2] = { [&](char val_ch) -> bool {
                if ('.' == val_ch)
                {
                    status = 3;
                }
                else if (!numeric_p(val_ch))
                {
                    return false;
                }

                return true;
            } };
        stat_funcs[3] = { [&](char val_ch) -> bool {
                if (!numeric_p(val_ch))
                {
                    return false;
                }

                status = 4;
                return true;
            } };
        stat_funcs[4] = { [&](char val_ch) -> bool {
                return numeric_p(val_ch);
            } };

        for (auto&& val_ch : val_str)
        {
            if (!stat_funcs[status](val_ch))
            {
                return false;
            }
        }

        return 2 == status ||
                4 == status;
    }

    static string trim_left(const string& val_str)
    {
        using FuncType = bool (*)(char);
        auto itor = std::find_if(val_str.begin(),
                                 val_str.end(),
                                 std::not1(std::function<bool(char)>(FuncType(StringUtils::whitespace_p))));
        return string(itor, val_str.end());
    }

    static string trim_right(const string& val_str)
    {
        using FuncType = bool (*)(char);
        auto itor = std::find_if(val_str.rbegin(),
                                 val_str.rend(),
                                 std::not1(std::function<bool(char)>(FuncType(StringUtils::whitespace_p))));
        return string(val_str.begin(), itor.base());
    }

    static string trim(const string& val_str)
    {
        return trim_left(trim_right(val_str));
    }
};

}
