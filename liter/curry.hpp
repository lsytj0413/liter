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

template <typename F>
auto curry(F f);

template <typename... T>
class Curry;

template <typename R, typename H, typename... TArgs>
class Curry<R(H, TArgs...)>
{
private:
    std::function<R(H, TArgs...)> m_fn;

public:
    template <typename Func>
    Curry(Func&& f) {
        m_fn = f;
    };

    auto operator()(H&& arg) {
        auto v = Curry([arg, this](TArgs... args) {
                return m_fn(arg, args...);
            });
        return curry(&v.operator());
    };
};

template <typename F>
auto curry_imp(F&& f, std::index_sequence<>) {
    return f;
};

template <typename F, size_t... I, typename std::enable_if<(sizeof...(I) > 0), void>::type>
auto curry_imp(F&& f, std::index_sequence<I...>) {
    using FT = function_traits<F>;
    return Curry<typename FT::result_type,
                 typename FT::template args<I>::type...
                 >(f);
};

template <typename F>
auto curry(F f) {
    using FT = function_traits<F>;
    return curry_imp(f, std::make_index_sequence<FT::arity>());
}

}
