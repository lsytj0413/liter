#include "matchTest.h"

#include <gtest/gtest.h>
#include <vector>
#include <string>
using std::string;
using std::vector;
#include <algorithm>

// #include <liter/algo/match.hpp>
// using namespace liter::algo;

// http://blog.csdn.net/v_july_v/article/details/7041827
class matcher
{
private:
    vector<int> m_next;

public:
    vector<int> test_gen_next(const string& pattern){
        auto size = pattern.size();

        m_next.resize(size);
        std::fill_n(std::begin(m_next), 0, size);
        m_next[0] = -1;

        int j = 0;
        int k = -1;
        while(j < size) {
            if (k == -1 || pattern[j] == pattern[k]) {
                ++j;
                ++k;
                m_next[j] = k;

                // if (pattern[j] != pattern[k]) {
                //     m_next[j] = k;
                // }
                // else {
                //     m_next[j] = m_next[k];
                // }
            }
            else {
                k = m_next[k];
            }
        }

        return m_next;
    };

    int match(const string& s, const string& pattern) {
        if(pattern.empty()) {
            return 0;
        }

        if (s.empty()) {
            return -1;
        }

        int i = 0;
        int j = 0;
        test_gen_next(pattern);

        while(i < s.size() && j < int(pattern.size())) {
            if (j == -1 || s[i] == pattern[j]) {
                ++i;
                ++j;
            }
            else {
                j = m_next[j];
            }
        }

        if (j == pattern.size()) {
            return i - j;
        }
        else {
            return -1;
        }
    }
};


TEST_F(MatchTest, testGenNext1)
{
    matcher m;

    auto v = m.test_gen_next("abab");
    EXPECT_EQ(v.size(), 4);

    vector<int> p = {-1, 0, 0, 1};

    for(int i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], p[i]);
    }
}


TEST_F(MatchTest, testGenNext2)
{
    matcher m;

    auto v = m.test_gen_next("abcdabd");
    EXPECT_EQ(v.size(), 7);

    vector<int> p = {-1, 0, 0, 0, 0, 1, 2};

    for(int i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], p[i]);
    }
}


TEST_F(MatchTest, testMatch)
{
    matcher m;

    auto i = m.match("abcdabd", "abd");
    EXPECT_EQ(i, 4);
}
