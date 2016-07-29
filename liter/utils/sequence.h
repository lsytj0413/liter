#pragma once

#include <tuple>



namespace liter
{

template <size_t... Seq>
struct sequence;

template <size_t End, size_t Beg = 0>
struct make_sequence
{
private:
    template <typename T, size_t... Seq>
    struct make_sequence_imp
    {

    };

    template <size_t Beg, size_t End, size_t... Seq>
    struct make_sequence_imp<sequence<Seq...>, Beg, End>
    {
        using type = typename make_sequence_imp<sequence<Seq..., Beg>, Beg + 1, End>::type;
    };

    template <size_t End, size_t... Seq>
    struct make_sequence_imp<sequence<Seq...>, End, End>
    {
        using type = sequence<Seq...>;
    };

public:
    using type = typename make_sequence_imp<sequence<>, Beg, End>::type;
};

}
