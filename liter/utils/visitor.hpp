#pragma once


namespace liter
{

template <typename... TArgs>
struct visitor;

template <typename T, typename... Rest>
struct visitor<T, Rest...> : visitor<Rest...>
{
    using visitor<Rest...>::visit;
    virtual void visit(const T&) = 0;
};

template <typename T>
struct visitor<T>
{
    virtual void visit(const T&) = 0;
};

}
