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

class matcher {
public:
    int match(const string& s, const string& pattern) {
        if (pattern.empty()) {
            return 0;
        }

        if (s.empty()) {
            return -1;
        }

        return match_imp(s, pattern);
    }

protected:
    virtual int match_imp(const string& s, const string& pattern) = 0;
};


class KPM_matcher : public matcher {
public:
    vector<int> test_gen_next(const string& pattern) {
        if (!pattern.empty()) {
            gen_next(pattern);
        }

        return m_next;
    }

protected:
    vector<int> m_next;

protected:
    virtual int match_imp(const string& s, const string& pattern) override final {
        gen_next(pattern);

        int i = 0;
        int j = 0;

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
    };

    void gen_next(const string& pattern) {
        auto size = pattern.size();
        fill_next(size);

        int j = 0;
        int k = -1;
        while(j < size) {
            if (k == -1 || pattern[j] == pattern[k]) {
                ++j;
                ++k;

                m_next[j] = calc_next_value(j, k, pattern);
            }
            else {
                k = m_next[k];
            }
        }
    }

    virtual int calc_next_value(int j, int k, const string& pattern) {
        return k;
    }
private:
    void fill_next(size_t size) {
        m_next.resize(size);
        std::fill_n(std::begin(m_next), 0, size);
        m_next[0] = -1;
    }
};


class KPM2_matcher final: public KPM_matcher{
protected:
    virtual int calc_next_value(int j, int k, const string& pattern) override final {
        if (pattern[j] != pattern[k]) {
            return k;
        }

        return m_next[k];
    };
};

using KPM1 = KPM_matcher;
using KPM2 = KPM2_matcher;
using KPM = KPM2;

}

}

