#pragma once

#include <string>
#include <vector>
#include <algorithm>
using std::vector;
using std::string;


namespace liter
{
namespace algo
{

class matcher
{
private:
    vector<int> m_next;

public:
    matcher(){};
    ~matcher(){};

    int match(const string& s, const string& pattern) {
        if (pattern.empty()) {
            return 0;
        }

        if (s.empty()) {
            return -1;
        }

        gen_next(pattern);

        int i = 0;
        int j = 0;
        while(i < s.size() && j < pattern.size()) {
            if (0 == j || s[i] == pattern[j]) {
                ++i;
                ++j;
            }
            else {
                j = m_next[j];
            }
        }

        if (j >= pattern.size()) {
            return i - pattern.size();
        }

        return -1;
    };

    vector<int> test_gen_next(const string& pattern) {
        if (!pattern.empty()) {
            gen_next(pattern);
        }

        return m_next;
    };

private:
    void gen_next(const string& pattern) {
        auto size = pattern.size();

        m_next.resize(size);
        std::fill_n(std::begin(m_next), 0, size);

        m_next[0] = -1;
        int j = 0;
        for (int i = 1; i < size; ++i)
        {
            j = m_next[i-1];
            while(j >= 0 && pattern[j + 1] != pattern[i]) {
                j = m_next[j];
            }

            if (pattern[j+1] == pattern[i]) {
                m_next[i] = j + 1;
            }
            else {
                m_next[i] = -1;
            }
        }

        // int i = 1;
        // int j = 0;
        // while(i < size) {
        //     if (0 == j || pattern[i] == pattern[j]) {
        //         m_next[i] = j;
        //         ++j;
        //         ++i;
        //     }
        //     else {
        //         j = m_next[j];
        //     }
        // }
    }
};

}
}

