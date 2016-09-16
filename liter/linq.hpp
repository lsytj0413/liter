#pragma once

#include <numeric>

#include <boost/algorithm/minmax_element.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/range.hpp>
#include <boost/range/join.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>

using namespace boost;
using namespace boost::adaptors;


namespace liter
{

template <typename F>
struct function_traits : public function_traits<decltype(&F::operator())>
{};

template <typename C, typename R, typename... TArgs>
struct function_traits<R(C::*)(TArgs...) const>
{
    using function = std::function<R(TArgs...)>;
};

template <typename F>
typename function_traits<F>::function
to_function(F& lambda){
    return static_cast<typename function_traits<F>::function>(lambda);
};



}
