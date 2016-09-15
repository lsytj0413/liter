#pragma once

#include <cstddef>


namespace liter
{

template <size_t... Seq>
struct sequence{};

template <size_t End, size_t Beg = 0, int step = 1>
struct make_sequence
{
private:
    template <typename T, size_t... Seq>
    struct make_sequence_imp
    {

    };

    template <size_t Beg0, size_t End0, size_t... Seq>
    struct make_sequence_imp<sequence<Seq...>, Beg0, End0>
    {
        using type = typename make_sequence_imp<sequence<Seq..., Beg0>, Beg0 + step, End0>::type;
    };

    template <size_t End0, size_t... Seq>
    struct make_sequence_imp<sequence<Seq...>, End0, End0>
    {
        using type = sequence<Seq...>;
    };

public:
    using type = typename make_sequence_imp<sequence<>, Beg, End>::type;
};

template <typename... Seq>
struct reverse_sequence_imp;

template <size_t... Seq1, size_t head, size_t... Seq2>
struct reverse_sequence_imp<sequence<head, Seq1...>, sequence<Seq2...>>
{
    using type = typename reverse_sequence_imp<sequence<Seq1...>,
                                               sequence<head, Seq2...>>::type;
};

template <size_t... Seq>
struct reverse_sequence_imp<sequence<>, sequence<Seq...>>
{
    using type = sequence<Seq...>;
};

template <typename... Seq>
struct reverse_sequence;

template <size_t... Seq>
struct reverse_sequence<sequence<Seq...>>
{
    using type = typename reverse_sequence_imp<sequence<Seq...>, sequence<>>::type;
};


}
