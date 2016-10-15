// @file visitor.hpp
// @brief 访问者模式实现模板
// @author
// @version
// @date

#pragma once


namespace liter
{

template <typename... TArgs>
struct visitor;

// @struct visitor
// @brief 访问方法定义
template <typename T, typename... Rest>
struct visitor<T, Rest...> : visitor<Rest...>
{
    using visitor<Rest...>::visit;
    virtual void visit(const T&) = 0;
};

// @struct visitor
// @brief 访问方法定义
template <typename T>
struct visitor<T>
{
    virtual void visit(const T&) = 0;
};

}
