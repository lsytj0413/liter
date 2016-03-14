#pragma once


/* 最大堆
 * 一种二叉堆，满足每个节点的值最多和其父节点一样大.
 * 一般使用与堆排序.
*/

#include <functional>

#include "algo/minmax_heap_imp.h"

namespace liter
{
namespace algo
{

template <typename T>
class max_heap : public minmax_heap_imp<T, std::greater<T>>
{
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
		return minmax();
    }

    value_type pop_max()
    {
		return pop_minmax();
    }
};

}
}
