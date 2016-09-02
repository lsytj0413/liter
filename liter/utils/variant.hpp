#pragma once

#include <typeindex>
#include <type_traits>


namespace liter
{

template <std::size_t arg, std::size_t... rest>
struct IntegerMax;

template <std::size_t arg>
struct IntegerMax<arg> : public std::integral_constant<std::size_t, arg>
{};

template <std::size_t arg1, std::size_t arg2, std::size_t... rest>
struct IntegerMax<arg1, arg2, rest...> :
            public std::integral_constant<std::size_t,
                                          arg1>=arg2 ? IntegerMax<arg1, rest...>::value : IntegerMax<arg2, rest...>::value>
{};

template <typename... TArgs>
struct MaxAlign : public std::integral_constant<int, IntegerMax<std::alignment_of<TArgs>::value...>::value>
{};

template <typename T, typename... List>
struct Contains;

template <typename T, typename Head, typename... Rest>
struct Contains<T, Head, Rest...> :
            public std::conditional<std::is_same<T, Head>::value,
                                    std::true_type,
                                    Contains<T, Rest...>>::type
{};

template <typename T>
struct Contains<T> : public std::false_type
{};

template <typename T, typename... List>
struct IndexOf;

template <typename T, typename Head, typename... Rest>
struct IndexOf<T, Head, Rest...>
{
    enum {
        value = IndexOf<T, Rest...>::value + 1
    };
};

template <typename T, typename... Rest>
struct IndexOf<T, T, Rest...>
{
    enum {
        value = 0
    };
};

template <typename T>
struct IndexOf<T>
{
    enum {
        value = -1
    };
};

template <int index, typename... Types>
struct At;

template <int index, typename First, typename... Types>
struct At<index, First, Types...>
{
    using type = typename At<index - 1, Types...>::type;
};

template <typename T, typename... Types>
struct At<0, T, Types...>
{
    using type = T;
};


}
