// @file hash.hpp
// @brief 一些hash函数实例
// @author
// @version
// @date

# pragma once

#include <cctype>


namespace liter
{

namespace algo
{

struct Identify {};
struct Thomas {};

template <typename T>
unsigned int hash(unsigned int);

// @brief 简单hash函数
template <>
unsigned int hash<Identify>(unsigned int key) {
    return key;
}

// @brief Thomas Wang's 32bit Mix Function
template <>
unsigned int hash<Thomas>(unsigned int key) {
    key += ~(key << 15);
    key ^= (key >> 10);
    key += (key << 3);
    key ^= (key >> 6);
    key += ~(key << 11);
    key ^= (key >> 16);

    return key;
}

// @brief Generic hash function, one from Bernstein, 大小写敏感
template <bool sensitive = true>
unsigned int hash(const unsigned char* buf, unsigned int len)
{
    unsigned int hash = 5381;

    while(len--) {
        // hash * 33 + c
        hash = ((hash << 5) + hash) + (*buf++);
    }
    return hash;
}

// @brief 大小写不敏感
template <>
unsigned int hash<false>(const unsigned char* buf, unsigned int len)
{
    unsigned int hash = 5381;

    while(len--) {
        // hash * 33 + c
        hash = ((hash << 5) + hash) + tolower(*buf++);
    }
    return hash;
}

}

}

