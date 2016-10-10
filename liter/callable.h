// @file callable.h
// @brief 可调用对象的参数解包示例
// @author
// @version
// @date

#pragma once

#include <tuple>
#include "liter/sequence.hpp"


namespace liter
{

namespace detail
{

template <typename... TArgs, size_t... Seq>
void functor_imp(std::tuple<TArgs...> args, sequence<Seq...>)
{
    std::get<0>(args)(std::get<Seq>(args)...);
};

template <typename... TArgs>
void functor_imp(std::tuple<TArgs...> args)
{
    functor_imp(args,
                typename make_sequence<std::tuple_size<decltype(args)>::value, 1>::type());
};

}

template <typename... TArgs>
void functor_call(TArgs... args)
{
    detail::functor_imp(std::tuple<TArgs...>(args...));
}

}
