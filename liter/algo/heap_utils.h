#pragma once

#include <iterator>


namespace liter
{

namespace algo
{

/// ������+
template <typename Iter>
Iter iter_advance(Iter itor, typename Iter::size_type i)
{
    std::advance(itor, i);
    return itor;
}

/// ������
template <typename Iter, typename Functor>
void build_heap(Iter first, Iter last, Functor f)
{
    auto heap_size = std::distance(first, last);
     for (auto i = heap_size/2; i >= 0; --i)
    {
        heapify_down(iter_advance(first, i), last, f);
    }
}

/// ����ƽ���
template <typename Iter, typename Functor>
void heapify_down(Iter first, Iter now, Iter last, Functor f)
{
    auto i = std::distance(first, now);
    auto l = left(i);
    auto r = right(i);
    auto cur = i;
    auto size = std::distance(first, last);

    if ((l < size) && f(*now, *(iter_advance(first, l))))
    {
        cur = l;
    }

    if ((r < size) && f(*(iter_advance(first, cur)), *(iter_advance(first, r))))
    {
        cur = r;
    }

    if (i != cur)
    {
        std::swap(*now, *(iter_advance(first, cur)));
        heapify_down(first, *(iter_advance(first, cur), last, f));
    }
}

/// ����ƽ���
template <typename Iter, typename Functor>
void heapify_up(Iter first, Iter now, Iter last, Functor f)
{
}

}

}
