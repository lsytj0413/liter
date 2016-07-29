#pragma once

#include <iterator>
/* 堆排序 O(nlgn)
 * 原地排序算法，在任何时候，数组中只有常数个元素存储在输入数组之外.
 * 堆：一种完全二叉树，除最后一层外所有层都是满的.
 * 表示堆的数组A是一个具有两个属性的对象：length[A]是数组中元素个数，heap_size[A]是存放在A中的堆元素个数，heap_size[A]<=length[A].树的根为A[0].
*/

namespace liter
{

namespace algo
{

/// 从大到小排序 -->> 需要使用最小堆
/// 从小到大排序 -->> 需要使用最大堆
template <typename Iter, typename Functor>
void heap_sort(Iter first, Iter last, Functor f)
{
    if(first == last)
    {
        return ;
    }

    build_heap(first, last, f);

    last = std::advance(last, -1);
    while(last != first)
    {
        std::swap(*first, *last);
        heapify_down(first, last, f);
    }
}

}

}
