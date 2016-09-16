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

template <typename R>
class linq
{
private:
    R m_linq_range;

public:
    using value_type = typename R::value_type;

public:
    linq(R& range) : m_linq_range(range)
    {};

    template <typename F>
    decltype(auto) where(const F& f){
        return linq<filtered_range<F, R>>(filter(m_linq_range, f));
    };

    template <typename F>
    decltype(auto) select(const F& f){
        auto fn = to_function(f);
        return linq<transformed_range<typename function_traits<F>::function, R>>(
            boost::adaptors::transform(m_linq_range, fn));
    };

    decltype(auto) begin() const {
        return std::begin(m_linq_range);
    };

    decltype(auto) end() const {
        return std::end(m_linq_range);
    };

    bool empty() const {
        return begin() == end();
    };

    decltype(auto) reverse() {
        return linq<boost::reversed_range<R>>(boost::adaptors::reverse(m_linq_range));
    };

    template <typename F>
    decltype(auto) first(const F& f){
        return std::find_if(begin(), end(), f);
    };

    template <typename F>
    decltype(auto) last(const F& f){
        return reverse().first();
    };

    template <typename F>
    decltype(auto) any(const F& f){
        return std::any_of(begin(), end(), f);
    };

    template <typename F>
    decltype(auto) all(const F& f){
        return std::all_of(begin(), end(), f);
    };

    template <typename F>
    void for_each(const F& f){
        std::for_each(begin(), end(), f);
    };
};

}
