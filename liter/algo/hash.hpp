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

template <typename T = Identify>
unsigned int hash(unsigned int key) {
    return key;
}

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


// Generic hash function, one from Bernstein
unsigned int hash(const unsigned char *buf, int len) {
    unsigned int hash = 5381;

    while(len--) {
        // hash * 33 + c
        hash = ((hash << 5) + hash) + (*buf++);
    }
    return hash;
}

// Generic hash function, one from Bernstein, insensitive
unsigned int hash_insensitive(const unsigned char *buf, int len) {
    unsigned int hash = 5381;

    while(len--) {
        // hash * 33 + c
        hash = ((hash << 5) + hash) + tolower(*buf++);
    }
    return hash;
}


}

}

