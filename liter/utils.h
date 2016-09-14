#pragma once

#include <utility>


namespace liter
{

template <typename T>
T variadic_sum(T&& v)
{
    return v;
}

template <typename T, typename... TArgs>
decltype(auto) variadic_sum(T&& v1, TArgs&&... args)
{
    return v1 + variadic_sum(std::forward<TArgs>(args)...);
}

}
