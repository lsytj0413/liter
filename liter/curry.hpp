// @file curry.hpp
// @brief 函数科里化实现模板
// @author
// @version
// @date

#pragma once

#include <functional>

#include "function_traits.hpp"


namespace liter
{

namespace detail
{

template <typename F>
auto curry(F f);

template <typename R, typename H, typename... TArgs>
class Curry
{
private:
    std::function<R(H, TArgs...)> m_fn;

public:
    template <typename Func>
    Curry(Func&& f) {
        m_fn = f;
    };

    auto operator()(H&& arg) {
        auto f = [arg, this](TArgs... args) {
            return m_fn(arg, args...);
        };

        return curry(f);
    };
};

template <typename F>
auto curry_imp(F f, std::index_sequence<>) {
    return f;
};

template <typename F,
          size_t... I,
          typename = typename std::enable_if<(sizeof...(I)>0)>::type
        >
auto curry_imp(F f, std::index_sequence<I...>) {
    using FT = function_traits<F>;
    return Curry<
        typename FT::return_type,
        typename FT::template args<0>::type,
        typename FT::template args<I+1>::type...
        >(f);
};

template <typename F>
auto curry(F f) {
    using FT = function_traits<F>;
    return curry_imp(f, std::make_index_sequence<FT::arity-1>());
}

}

template <typename F>
auto curry(F f) {
    using FT = function_traits<F>;
    static_assert(FT::arity>0, "The function must have at least one argument.");
    return detail::curry(f);
};

// template <typename FuncT>
// auto curry(FuncT);

// template <
//     typename... LeftArgs,
//     typename FuncT,
//     typename FirstArg,
//     typename = typename std::enable_if<std::is_reference<FirstArg>::value>::type,
//     typename = void
// >
// auto foward_first(FuncT func, FirstArg&& first)
// {
//     return curry([func, &first](LeftArgs... args) mutable
//         -> decltype(func(std::forward<FirstArg>(first), std::forward<LeftArgs>(args)...))
//     {
//         return func(std::forward<FirstArg>(first), std::forward<LeftArgs>(args)...);
//     });
// }

// template <
//     typename... LeftArgs,
//     typename FuncT,
//     typename FirstArg,
//     typename = typename std::enable_if<!std::is_reference<FirstArg>::value>::type
// >
// auto foward_first(FuncT func, FirstArg&& first)
// {
//     return curry([func, first](LeftArgs... args) mutable -> decltype(func(first, std::forward<LeftArgs>(args)...))
//     {
//         return func(first, std::forward<LeftArgs>(args)...);
//     });
// }

// template <typename FuncT>
// auto curryImpl(FuncT func, std::index_sequence<>)
// {
//     return func;
// }

// template <typename FuncT, size_t... Is, typename = typename std::enable_if<(sizeof...(Is) > 0)>::type>
// auto curryImpl(FuncT func, std::index_sequence<Is...>)
// {
//     return [func](typename function_traits<FuncT>::template args<0>::type first) mutable
//     {
//         return foward_first<typename function_traits<FuncT>::template args<Is + 1>::type...>
//         (func, std::forward<typename function_traits<FuncT>::template args<0>::type>(first));
//     };
// }

// template <typename FuncT>
// auto curry(FuncT func)
// {
//     return curryImpl(func, std::make_index_sequence<function_traits<FuncT>::arity - 1>());
// }

}
