#pragma once

#include <tuple>
#include "liter/utils/sequence.h"


namespace liter
{

namespace detail
{

template <typename... TArgs, size_t... Seq>
void functor_imp(std::tuple<TArgs...> args, sequence<Seq...>)
{
    std::get<0>(args)(std::tuple<Seq>(args)...);
}

template <typename... TArgs>
void functor_imp(std::tuple<TArgs...> args)
{
    functor_imp(args,
                make_sequence<std::tuple_size<decltype(args)>::value, 1>::type());
}

}

template <typename... TArgs>
void functor_call(TArgs... args)
{
    detail::functor_imp(std::tuple<TArgs...>(args...));
}

}
