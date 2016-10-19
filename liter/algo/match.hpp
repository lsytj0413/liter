// @file match.hpp
// @brief 字符串查找算法, 详细描述可查看 http://blog.csdn.net/v_july_v/article/details/7041827
// @author
// @version
// @date

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

// @class matcher
// @brief 基类
class matcher {
public:
    // @function
    // @brief 子串查找
    // @param s: 源字符串
    // @param pattern: 模式串, 待查找的子串
    // @return int, 未找到则返回-1
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

// @class KPM_matcher
// @brief KPM算法查找, 未优化的next计算
// 算法描述:
// 设有文本串S, 模式串P, 在S中查找P出现的位置.
// 假设现在S匹配到未知i, P匹配到位置j, 则有:
// (1) 如果 -1==j, 或者 S[i]==P[j], 则令++i,++j, 继续进行匹配
// (2) 如果不满主(1), 则i不变, j = next[j], 重新进行匹配. 相当于P向右移动j-next[j]个字符
// next求解方式:
// (1) 求出P的前缀后缀最长公共元素长度, 例如 abab={0, 0, 1, 2}
//     利用该表进行匹配, 则向右移动的位数= 已匹配字符数-失配字符的上一个字符对应的最大长度
// (2) 将上一步的结果右移一位, 首位填-1, 得到 {-1, 0, 0, 1}
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
                // 从s的下一个字符进行匹配
                // 若-1==j, 则相当于从s[i+1], p[0]重新进行匹配
                // 若-1!=j, 则相当于此次匹配成功, 继续进行下一个字符的匹配
                ++i;
                ++j;
            }
            else {
                // 匹配失败, 向右滑动模式串
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

    // @function
    // @brief 计算next数组
    // 描述:
    // 当next[j]=k时, 求 next[j+1]:
    // (1) 若p[k]==p[j]或者-1==k, 则next[j+1]=k+1
    // (2) 若p[k]!=p[j], 则令k=next[k], 继续进行计算
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

// @class KPM_matcher
// @brief KPM算法查找, 优化的next计算
// 优化描述:
// 例如, 常规next求得 abab={-1,0,0,1}
// 当在j=3处失配时, 模式串右移3-next[3]=2位, 从j=1处继续匹配.
// 但p[1]==p[3], 而s[i]!=p[3], 所以这一次的匹配必定失败,
// 则在求取next时, 可以通过n不允许p[j]==p[next[j]]来避免这种匹配失败
// 优化结果. abab={-1,0,-1,0}
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

