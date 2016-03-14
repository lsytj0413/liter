#pragma once

/* 最小堆
 * 一种二叉堆，满足每个节点的值最多和其子节点一样大.
 * 一般使用与优先队列.
*/

#include <functional>

#include "algo/minmax_heap_imp.h"

namespace liter
{
namespace algo
{

	template <typename T>
	class min_heap : public minmax_heap_imp<T, std::less<T>>
	{
	public:
		min_heap()
			: minmax_heap_imp<T, std::less<T>>()
		{};

		min_heap(size_type n, const value_type& val)
			: minmax_heap_imp<T, std::less<T>>(n, val)
		{
			};

		template <typename InputIter>
		min_heap(InputIter first, InputIter last)
			: minmax_heap_imp<T, std::less<T>>(first, last)
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
