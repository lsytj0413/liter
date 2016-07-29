#pragma once

#include <functional>
#include "liter/algo/minmax_heap_imp.h"

/* 最大堆
 * 一种二叉堆，满足每个节点的值最多和其父节点一样大.
 * 一般使用与堆排序.
*/
namespace liter
{

namespace algo
{

template <typename T>
class max_heap : public minmax_heap_imp<T, std::greater<T>>
{
public:
    using ParentType = minmax_heap_imp<T, std::greater<T>>;
    using size_type = typename ParentType::size_type;
    using value_type = typename ParentType::value_type;

public:
    max_heap()
		: minmax_heap_imp<T, std::greater<T>>()
    {};

    max_heap(size_type n, const value_type& val)
		: minmax_heap_imp<T, std::greater<T>>(n, val)
    {
    };

    template <typename InputIter>
    max_heap(InputIter first, InputIter last)
		: minmax_heap_imp<T, std::greater<T>>(first, last)
    {
    };

    value_type max() const
    {
        return this->minmax();
    }

    value_type pop_max()
    {
		    return this->pop_minmax();
    }
};

}

}
