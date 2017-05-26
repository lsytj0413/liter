#include "hashTest.h"

#include <gtest/gtest.h>

#include <vector>
#include <map>
#include <string>
using std::vector;
using std::map;
using std::string;


#include <liter/algo/hash.hpp>
using namespace liter::algo;


TEST_F(HashTest, testHashIndentify)
{
    vector<unsigned int> v = {
        0,
        5,
        99,
        199,
        23423423
    };
    for (auto&& i : v) {
        EXPECT_EQ(i, hash<Identify>(i));
    }
}

TEST_F(HashTest, testHashThomas)
{
    map<unsigned int, unsigned int> m = {
        {0, 1177991625},
        {3, 737174223},
        {100, 4292537205},
        {199, 2623574505},
        {342345, 1119757213}
    };

    for (auto&& p : m) {
        EXPECT_EQ(p.second, hash<Thomas>(p.first));
    }
}

TEST_F(HashTest, testHashBernsteinSensitive)
{
    map<string, unsigned int> m = {
        {"f1342342", 3566475694},
        {"fsfDSSF4234", 3321410817},
        {"F1342342", 1280933262},
        {"2346499", 2271350426},
        {"fjjFF", 259043307},
        {"fjjff", 259044395},
        {"", 5381}
    };

    for (auto&& p : m) {
        EXPECT_EQ(p.second, hash((const unsigned char*)p.first.c_str(), p.first.size()));
    }
}

TEST_F(HashTest, testHashBernsteinInsensitive)
{
    map<string, unsigned int> m = {
        {"f1342342", 3566475694},
        {"fsfDSSF4234", 979570049},
        {"F1342342", 3566475694},
        {"2346499", 2271350426},
        {"fjjFF", 259044395},
        {"fjjff", 259044395},
        {"", 5381}
    };

    for (auto&& p : m) {
        EXPECT_EQ(p.second, hash<false>((const unsigned char*)p.first.c_str(), p.first.size()));
    }
}
