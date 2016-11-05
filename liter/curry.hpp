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

// @function
// @brief 将函数转化为科里化表示
// @param f: 函数
// @return Curry类的对象
template <typename F>
auto curry(F f);


// @class Curry
// @brief 函数科里化的表示对象
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

    // @function
    // @brief 调用函数
    // @param arg: 参数
    // @return 新的科里化表示对象或者被科里化函数的调用结果
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

// @function
// @brief 将函数转化为科里化表示
// @param f: 函数
// @return Curry类的对象
template <typename F>
auto curry(F f) {
    using FT = function_traits<F>;
    static_assert(FT::arity>0, "The function must have at least one argument.");
    return detail::curry(f);
};

}
