#pragma once

/*
  最大/最小堆的公共实现.
  在最大/最小堆的实现过程中，基本只有比较函数的不同而已。故将此部分提出.
*/

#include <vector>
#include <iterator>
#include <exception>

#include "algo/tree_utils.h"

namespace liter
{
namespace algo
{
template <typename T, typename Functor>
class minmax_heap_imp
{
  public:
    using iterator = typename std::vector<T>::iterator;
    using size_type = typename std::vector<T>::size_type;
    using value_type = typename std::vector<T>::value_type;

  private:
    std::vector<T> m_array;
    Functor m_functor;

  public:
    minmax_heap_imp()
    {};

    minmax_heap_imp(size_type n, const value_type& val)
            : m_array(n, val)
    {};

    template <typename InputIter>
    minmax_heap_imp(InputIter first, InputIter last)
            : m_array(first, last)
    {
        build_heap();
    }

    size_type size() const
    {
        return m_array.size();
    }

    bool empty() const
    {
        return m_array.empty();
    }

	void set(size_type i, const value_type& val)
	{
		if (i >= size())
		{
			throw out_of_range("Out of size.");
		}


		if (m_functor(val, m_array[i]))
		{
			// 如果是最大堆, 则 val > m_array[i]
			// 如果是最小堆, 则 val < m_array[i]
			// 在这种情况下都需要向上调整
			m_array[i] = val;
			minmax_heapify_up(m_array.begin(), iter_at(i));
		}
		else
		{
			m_array[i] = val;
			minmax_heapify_down(m_array.begin(), iter_at(i));
		}
	}

	void insert(const value_type& val)
	{
		m_array.push_back(val);
		minmax_heapify_up(m_array.begin(), iter_at(size() - 1));
	}

  protected:
    value_type minmax() const
    {
        if (!empty())
        {
            return m_array[0];
        }

        throw std::out_of_range("empty heap.");
    }

    value_type pop_minmax()
    {
        auto val = minmax();
        std::swap(m_array[0], m_array[size() -1]);
        m_array.pop_back();

        minmax_heapify_down(m_array.begin(), m_array.begin());
        return val;
    }

  private:
    void build_heap()
    {
        auto heap_size = size();
        for (int i = heap_size/2; i >= 0; --i)
        {
            minmax_heapify_down(m_array.begin(), iter_at(i));
        }
    }

    void minmax_heapify_down(iterator first, iterator now)
    {
        auto i = std::distance(first, now);
        auto l = left(i);
        auto r = right(i);
        auto cur = i;

        if ((l < size()) && m_functor(m_array[l], m_array[i]))
        {
            // 满足调整的条件
            cur = l;
        }

        if ((r < size()) && m_functor(m_array[r], m_array[cur]))
        {
            cur = r;
        }

        if (i != cur)
        {
            // 调整
            std::swap(m_array[i], m_array[cur]);
            minmax_heapify_down(first, iter_at(cur)); // 递归
        }
    }

    void minmax_heapify_up(iterator first, iterator now)
    {
        auto i = std::distance(first, now);
        auto p = parent(i);
        while((i > 0) && (m_functor(m_array[i], m_array[p])))
        {
            std::swap(m_array[i], m_array[p]);
            i = parent(i);
            p = parent(i);
        }
    }

    iterator iter_at(size_type i)
    {
        auto iter = m_array.begin();
        std::advance(iter, i);
        return iter;
    }

};
}
}
