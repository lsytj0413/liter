#pragma once

#include <cstddef>


namespace liter
{

template <size_t... Seq>
struct sequence;

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

}
