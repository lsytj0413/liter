// @file tuple_helper.hpp
// @brief 一些std::tuple的帮助函数
// @author
// @version
// @date

#pragma once

#include <tuple>
#include <utility>

#include <liter/sequence.hpp>


namespace liter
{

namespace detail
{

template <typename T, typename U>
typename std::enable_if<std::is_convertible<T, U>::value || std::is_convertible<U, T>::value, bool>::type
        compare(T&& t, U&& u)
{
    return t == u;
};

bool compare(...)
{
    return false;
};

template <int I, typename T, typename... TArgs>
struct find_index
{
    static int call(std::tuple<TArgs...> const& t, T&& val)
    {
        return (compare(std::get<I-1>(t), val) ? I-1 :
                find_index<I-1, T, TArgs...>::call(t, std::forward<T>(val)));
    };
};

template <typename T, typename... TArgs>
struct find_index<0, T, TArgs...>
{
    static int call(std::tuple<TArgs...> const& t, T&& val)
    {
        return compare(std::get<0>(t), val) ? 0 : -1;
    };
};

}

// @function
// @brief 从std::tuple中查找某个值的索引
// @param t: tuple
// @param val: 需要查找的值
// @return int, 未找到时返回-1
template <typename T, typename... TArgs>
int find_index(std::tuple<TArgs...> const& t, T&& val)
{
    return detail::find_index<sizeof...(TArgs), T, TArgs...>::call(t, std::forward<T>(val));
}

namespace detail
{

template <typename Func, typename Last>
void apply_impl(Func&& fn, Last&& last){
    fn(std::forward<Last>(last));
};

template <typename Func, typename First, typename... Rest>
void apply_impl(Func&& fn, First&& first, Rest&&... rest){
    fn(std::forward<First>(first));
    apply_impl(std::forward<Func>(fn), std::forward<Rest>(rest)...);
};

template <typename Func, size_t... Indexes, typename... TArgs>
void apply_helper(Func&& fn, std::tuple<TArgs...>& tp, liter::sequence<Indexes...>){
    apply_impl(std::forward<Func>(fn), std::forward<TArgs>(std::get<Indexes>(tp))...);
};

}

// @function
// @brief 对tuple中的每个值应用 fn 函数
// @param fn: 函数
// @param tp: tuple
// @return
template <typename Func, typename Tuple>
void apply(Func&& fn, Tuple&& tp){
    detail::apply_helper(std::forward<Func>(fn),
                         std::forward<Tuple>(tp),
                         typename make_sequence<std::tuple_size<typename std::remove_reference<Tuple>::type>::value>::type());
};

template <typename... TArgs, size_t... Indexes>
decltype(auto) reverse_imp(const std::tuple<TArgs...>& tp, sequence<Indexes...>&&){
    return std::make_tuple(std::get<Indexes>(tp)...);
};

// @function
// @brief 反转tuple
// @param tp: tuple
// @return std::tuple, 反转的结果
template <typename... TArgs>
decltype(auto) reverse(const std::tuple<TArgs...>& tp){
    return reverse_imp(tp,
                       typename reverse_sequence<typename make_sequence<sizeof...(TArgs)>::type>::type()
                       );
};

namespace detail
{

template <int N, typename T1, typename T2>
using pair_type = std::pair<typename std::tuple_element<N, T1>::type,
                            typename std::tuple_element<N, T2>::type>;

template <int N, typename T1, typename T2>
pair_type<N, T1, T2> pair(const T1& tp1, const T2& tp2){
    return std::make_pair(std::get<N>(tp1),
                          std::get<N>(tp2)
                          );
};

template <size_t... Indexes, typename T1, typename T2>
decltype(auto) pairs_helper(sequence<Indexes...>, const T1& tp1, const T2& tp2){
    return std::make_tuple(pair<Indexes>(tp1, tp2)...);
};

}

// @function
// @brief 合并tuple
// @param tp1: tuple
// @param tp2: tuple
// @return std::tuple, 合并的结果
template <typename T1, typename T2>
decltype(auto) zip(T1&& tp1, T2&& tp2){
    using U1 = typename std::remove_reference<T1>::type;
    using U2 = typename std::remove_reference<T2>::type;
    static_assert(std::tuple_size<U1>::value == std::tuple_size<U2>::value,
                  "tuples should be the same size."
                  );

    return detail::pairs_helper(typename make_sequence<std::tuple_size<U1>::value>::type(),
                                std::forward<T1>(tp1),
                                std::forward<T2>(tp2)
                                );
};

}
