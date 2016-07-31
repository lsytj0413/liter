#pragma once

#include <string>
#include <cctype>
#include <functional>
#include <algorithm>
using std::string;
using std::function;


namespace liter
{

bool numeric_p(char val_ch)
{
    return isdigit(val_ch);
}

bool whitespace_p(char val_ch)
{
    return ' ' == val_ch ||
            '\t' == val_ch;
}

bool whitespace_p(const string& val_str)
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

bool int_p(const string& val_str)
{
		struct SStatFunc
		{
        std::function<bool(char, int&)> stat_func;
		};

		std::array<SStatFunc, 3> stat_funcs;
		stat_funcs[0] = {[](char val_ch, int& status) -> bool{
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
		stat_funcs[1] = {[](char val_ch, int& status) -> bool {
            if (!numeric_p(val_ch))
            {
                return false;
            }

            status = 2;
            return true;
        } };
		stat_funcs[2] = {[](char val_ch, int& status) -> bool {
            if (!numeric_p(val_ch))
            {
                return false;
            }
            return true;
        } };

		int status = 0;
		for (auto&& val_ch : val_str)
		{
        if (!stat_funcs[status].stat_func(val_ch, status))
        {
            return false;
        }
		}

		return 2 == status;
}

bool float_p(const string& val_str)
{
		struct SStatFunc
		{
        std::function<bool(char, int&)> stat_func;
		};

		std::array<SStatFunc, 5> stat_funcs;
		stat_funcs[0] = { [](char val_ch, int& status) -> bool {
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
		stat_funcs[1] = { [](char val_ch, int& status) -> bool {
            if (!numeric_p(val_ch))
            {
                return false;
            }

            status = 2;
            return true;
        } };
		stat_funcs[2] = { [](char val_ch, int& status) -> bool {
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
		stat_funcs[3] = { [](char val_ch, int& status) -> bool {
            if (!numeric_p(val_ch))
            {
                return false;
            }

            status = 4;
            return true;
        } };
		stat_funcs[4] = { [](char val_ch, int& status) -> bool {
            return numeric_p(val_ch);
        } };

		int status = 0;
		for (auto&& val_ch : val_str)
		{
        if (!stat_funcs[status].stat_func(val_ch, status))
        {
            return false;
        }
		}

		return 2 == status ||
            4 == status;
}

string trim_left(const string& val_str)
{
    using FuncType = bool (*)(char);
		auto itor = std::find_if(val_str.begin(),
                             val_str.end(),
                             std::not1(std::function<bool(char)>(FuncType(whitespace_p))));
		return string(itor, val_str.end());
}

string trim_right(const string& val_str)
{
    using FuncType = bool (*)(char);
		auto itor = std::find_if(val_str.rbegin(),
                             val_str.rend(),
                             std::not1(std::function<bool(char)>(FuncType(whitespace_p))));
		return string(val_str.begin(), itor.base());
}

string trim(const string& val_str)
{
		return trim_left(trim_right(val_str));
}

}
