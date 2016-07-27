#pragma once

/* ������ O(nlgn)
 * ԭ�������㷨�����κ�ʱ��������ֻ�г�����Ԫ�ش洢����������֮��.
 * �ѣ�һ����ȫ�������������һ�������в㶼������.
 * ��ʾ�ѵ�����A��һ�������������ԵĶ���length[A]��������Ԫ�ظ�����heap_size[A]�Ǵ����A�еĶ�Ԫ�ظ�����heap_size[A]<=length[A].���ĸ�ΪA[0].
*/

namespace liter
{
namespace algo
{
namespace detail
{

}
/// �Ӵ�С���� -->> ��Ҫʹ����С��
/// ��С�������� -->> ��Ҫʹ������

template <typename Iter, typename Functor>
void heap_sort(Iter first, Iter last, Functor f)
{
    if(first == last)
    {
        return ;
    }

    build_heap(first, last, f);

    last = std::adjust(last, -1);
    while(last != first)
    {
        std::swap(*first, *last);
        heapify_down(first, last, f);
    }    
}

}
}
