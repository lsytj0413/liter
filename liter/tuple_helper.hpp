#pragma once

#include <tuple>
#include <iostream>
using std::cout;
using std::endl;

#include <liter/sequence.h>


namespace liter
{

template <typename Tuple, std::size_t N>
struct TuplePrinter
{
    static void print(const Tuple& t)
    {
        TuplePrinter<Tuple, N-1>::print(t);
        cout << ", " << std::get<N-1>(t);
    };
};

template <typename Tuple>
struct TuplePrinter<Tuple, 1>
{
    static void print(const Tuple& t)
    {
        cout << std::get<0>(t);
    };
};

template <typename... TArgs>
void PrintTuple(const std::tuple<TArgs...>& t)
{
    cout << "(";
    TuplePrinter<decltype(t), sizeof...(TArgs)>::print(t);
    cout << ")" << endl;
}

namespace detail
{

template <typename T>
void Print(T& t)
{
    cout << t << endl;
};

template <typename T, typename... TArgs>
void Print(T& t, TArgs&... args)
{
    cout << t << endl;
    Print(args...);
};

template <typename Tuple, int... Indexes>
void transform(sequence<Indexes...>& in, Tuple& tp)
{
    Print(std::get<Indexes>(tp)...);
};

}

template <typename... TArgs>
void PrintTuple1(const std::tuple<TArgs...>& t)
{
    detail::transform(make_sequence<std::tuple_size<decltype(t)>::value>::type(), t);
}

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

template <typename T, typename... TArgs>
int find_index(std::tuple<TArgs...> const& t, T&& val)
{
    return detail::find_index<sizeof...(TArgs), T, TArgs...>::call(t, std::forward<T>(val));
}

template <size_t k, typename Tuple>
typename std::enable_if<(k==std::tuple_size<Tuple>::value)>::type
GetArgByIndex(size_t index, Tuple& tp)
{
    throw std::invalid_argument("arg index out of range");
};

template <size_t k=0, typename Tuple>
typename std::enable_if<(k<std::tuple_size<Tuple>::value)>::type
GetArgByIndex(size_t index, Tuple& tp)
{
    if (k == index)
    {
        cout << std::get<k>(tp) << endl;
    }
    else
    {
        GetArgByIndex<k+1>(index, tp);
    }
};

template <typename Arg>
void GetArgByIndex1(int index, std::tuple<Arg>& tp)
{
    cout << std::get<0>(tp) << endl;
};

template <typename Arg, typename... TArgs>
void GetArgByIndex1(int index, std::tuple<Arg, TArgs...>& tp)
{
    if (index < 0 || index >= std::tuple_size<decltype(tp)>::value)
    {
        throw std::invalid_argument("index is not valid");
    }

    if (index > 0)
    {
        GetArgByIndex1(index-1, (std::tuple<TArgs...>&)tp);
    }
    else
    {
        cout << std::get<0>(tp) << endl;
    }
};

namespace detail
{

template <typename Func, typename Last>
void for_each_impl(Func&& fn, Last&& last){
    fn(std::forward<Last>(last));
};

template <typename Func, typename First, typename... Rest>
void for_each_impl(Func&& fn, First&& first, Rest&&... rest){
    fn(std::forward<First>(first));
    for_each_impl(std::forward<Func>(fn), std::forward<Rest>(rest)...);
};

template <typename Func, int... Indexes, typename... TArgs>
void for_each_helper(Func&& fn, sequence<Indexes...>, std::tuple<TArgs...>&& tp){
    for_each_impl(std::forward<Func>(fn), std::forward<TArgs>(std::get<Indexes>(tp))...);
};

}

template <typename Func, typename Tuple>
void tp_for_each(Func&& fn, Tuple&& tp){
    for_each_helper(std::forward<Func>(fn),
                    typename make_sequence<std::tuple_size<Tuple>::value>::type(),
                    std::forward<Tuple>(tp));
};


template <typename... TArgs, int... Indexes>
decltype(auto) reverse_imp(std::tuple<TArgs...>&& tp, sequence<Indexes...>&&){
    return std::make_tuple(std::get<Indexes>(std::forward<std::tuple<TArgs>>(tp))...);
};

template <typename... TArgs>
decltype(auto) reverse(std::tuple<TArgs...>&& tp){
    return reverse_imp(std::forward<std::tuple<TArgs...>>(tp),\
                       typename make_sequence<0, std::tuple_size<decltype(tp)>::value, -1>::type()
                       );
};

template <typename F, typename... TArgs, int... Indexes>
decltype(auto) apply(F&& fn, std::tuple<TArgs...>&& tp, sequence<Indexes...>&&){
    return apply(std::forward<F>(fn),
                 std::forward<TArgs>(std::get<Indexes>(tp))...
                 );
};

template <typename F, typename... TArgs>
decltype(auto) apply(F&& fn, std::tuple<TArgs...>&& tp){
    return apply(std::forward<F>(fn),
                 std::forward<std::tuple<TArgs...>>(tp),
                 typename make_sequence<std::tuple_size<decltype(tp)>::value>::type()
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

template <int... Indexes, typename T1, typename T2>
decltype(auto) pairs_helper(sequence<Indexes...>, const T1& tp1, const T2& tp2){
    return std::make_tuple(pair<Indexes>(tp1, tp2)...);
};

}

template <typename T1, typename T2>
decltype(auto) zip(T1&& tp1, T2&& tp2){
    static_assert(std::tuple_size<T1>::value == std::tuple_size<T2>::value,
                  "tuples should be the same size."
                  );

    return detail::pairs_helper(typename make_sequence<std::tuple_size<T1>::value>::type(),
                                std::forward<T1>(tp1),
                                std::forward<T2>(tp2)
                                );
};

}
