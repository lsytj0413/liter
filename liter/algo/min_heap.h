#pragma once

#include <functional>
#include "liter/algo/minmax_heap_imp.h"

/* ��С��
 * һ�ֶ���ѣ�����ÿ���ڵ��ֵ�������ӽڵ�һ����.
 * һ��ʹ�������ȶ���.
*/

namespace liter
{

namespace algo
{

template <typename T>
class min_heap : public minmax_heap_imp<T, std::less<T>>
{
public:
    using ParentType = minmax_heap_imp<T, std::less<T>>;
    using size_type = typename ParentType::size_type;
    using value_type = typename ParentType::value_type;

public:
    min_heap()
            : minmax_heap_imp<T, std::less<T>>()
    {};

    min_heap(size_type n, const value_type& val)
            : minmax_heap_imp<T, std::less<T>>(n, val)
    {};

    template <typename InputIter>
            min_heap(InputIter first, InputIter last)
            : minmax_heap_imp<T, std::less<T>>(first, last)
    {};

    value_type min() const
    {
        return this->minmax();
    }

    value_type pop_min()
    {
        return this->pop_minmax();
    }
};

}

}
