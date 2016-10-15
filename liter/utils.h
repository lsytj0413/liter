// @file utils.h
// @brief 一些工具
// @author
// @version
// @date

#pragma once

#include <utility>


namespace liter
{

// @function
// @brief 求和
// @param v: 值
// @return T
template <typename T>
T variadic_sum(T&& v) {
    return v;
}

// @function
// @brief 求和
// @param v1: 值
// @param args: 可变参数列表, 值
// @return T
template <typename T, typename... TArgs>
auto variadic_sum(T&& v1, TArgs&&... args) {
    return v1 + variadic_sum(std::forward<TArgs>(args)...);
}

}
