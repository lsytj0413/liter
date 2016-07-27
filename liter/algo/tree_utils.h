#pragma once

/* utils
 * ������Ĳ����Ĺ���.
*/

namespace liter
{
namespace algo
{
namespace detail
{
// ��ȡ���ڵ�index, ��1��ʼ
inline size_t parent_imp(size_t i)
{
    return i/2;
}

// ��ȡ���ӽڵ�index����1��ʼ
inline size_t left_imp(size_t i)
{
    return 2*i;
}

// ��ȡ���ӽڵ�index����1��ʼ
inline size_t right_imp(size_t i)
{
    return 2*i + 1;
}
}

// ��ȡ���ڵ�index����0��ʼ
inline size_t parent(size_t i)
{
    return detail::parent_imp(i + 1) - 1;
}

// ��ȡ���ӽڵ�index����0��ʼ
inline size_t left(size_t i)
{
    return detail::left_imp(i + 1) - 1;
}

// ��ȡ���ӽڵ�index����0��ʼ
inline size_t right(size_t i)
{
    return detail::right_imp(i + 1) - 1;
}

}
}
