#include "hashTest.h"

#include <gtest/gtest.h>

#include <vector>
#include <map>
using std::vector;
using std::map;


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
        EXPECT_EQ(i, hash(i));
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
