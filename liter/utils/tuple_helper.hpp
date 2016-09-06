#pragma once

#include <tuple>
#include <iostream>
using std::cout;
using std::endl;

#include <liter/utils/sequence.h>


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


}
