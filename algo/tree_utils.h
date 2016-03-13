#pragma once

/* utils
 * 针对树的操作的工具.
*/

namespace liter
{
namespace algo
{
namespace detail
{
// 获取父节点index, 从1开始
inline size_t parent_imp(size_t i)
{
    return i/2;
}

// 获取左子节点index，从1开始
inline size_t left_imp(size_t i)
{
    return 2*i;
}

// 获取右子节点index，从1开始
inline size_t right_imp(size_t i)
{
    return 2*i + 1;
}
}

// 获取父节点index，从0开始
inline size_t parent(size_t i)
{
    return parent_imp(i + 1) - 1;
}

// 获取左子节点index，从0开始
inline size_t left(size_t i)
{
    return left_imp(i + 1) - 1;
}

// 获取右子节点index，从0开始
inline size_t right(size_t i)
{
    return right_imp(i + 1) - 1;
}

}
}
