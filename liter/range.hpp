// @file range.hpp
// @brief 生成范围序列
// @author
// @version
// @date

#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>
using std::logic_error;


namespace liter
{

namespace detail
{

template <typename T>
class RangeIterator
{
public:
    using value_type = T;
    using size_type = size_t;

    RangeIterator(value_type curr_val, value_type end_val, value_type step_val)
            : m_value(curr_val)
            , m_step(step_val)
            , m_end(end_val)
    {};

    value_type operator*() const {
        if (m_value >= m_end) {
            throw logic_error("can't get value of end iterator.");
        }

        return m_value;
    };

    bool operator!=(const RangeIterator& rhs) const {
        return m_value != rhs.m_value;
    };

    RangeIterator& operator++(void) {
        m_value += m_step;
        if (m_value > m_end) {
            m_value = m_end;
        }

        return (*this);
    };

private:
    value_type m_value;
    const value_type m_end;
    const value_type m_step;
};

}

// @class RangeImp
// @brief range实现类
template <typename T>
class RangeImp
{
public:
    using value_type = T;
    using reference = const value_type&;
    using const_reference = const value_type&;
    using iterator = const detail::RangeIterator<value_type>;
    using const_iterator = const detail::RangeIterator<value_type>;
    using size_type = typename detail::RangeIterator<value_type>::size_type;

    RangeImp(value_type begin_val, value_type end_val, value_type step_val)
            : m_begin(begin_val)
            , m_end(end_val)
            , m_step(step_val)
            , m_max_count(get_adjusted_count())
    {};

    // @function
    // @brief 个数
    // @return RangeImp::size_type
    size_type size() const {
        return m_max_count;
    };

    // @function
    // @brief 起始迭代器
    // @return RangeImp::const_iterator
    const_iterator begin() const {
        return {m_begin, m_end, m_step};
    };

    // @function
    // @brief 结束迭代器
    // @return RangeImp::const_iterator
    const_iterator end() const {
        return {m_end, m_end, m_step};
    };

private:
    const value_type m_begin;
    const value_type m_end;
    const value_type m_step;
    const value_type m_max_count;

private:
    size_type get_adjusted_count() const {
        if (m_step > 0 && m_begin >= m_end) {
            throw logic_error("End value must be greator than begin value.");
        }
        else if(m_step < 0 && m_begin <= m_end) {
            throw logic_error("End value must be less than begin value.");
        }
        else if(0 == m_step) {
            throw logic_error("Step value must be non-zero.");
        }

        size_type x = static_cast<size_type>((m_end - m_begin) / m_step);
        if (m_begin + (m_step * x) != m_end) {
            ++x;
        }
        return x;
    }
};

// @function
// @brief range包装函数
// @param end: 结束值
// @return RangeImp
template <typename T>
RangeImp<T> range(T&& end) {
    return {{}, std::forward<T>(end), 1};
}

// @function
// @brief range包装函数
// @param begin: 起始值
// @param end: 结束值
// @return RangeImp
template <typename T>
RangeImp<T> range(T&& begin, T&& end) {
    return {std::forward<T>(begin), std::forward<T>(end), 1};
}

// @function
// @brief range包装函数
// @param begin: 起始值
// @param end: 结束值
// @param step: 步长
// @return RangeImp
template <typename T, typename U>
auto range(T&& begin, T&& end, U&& step) {
    using return_type = RangeImp<decltype(begin + step)>;
    return return_type(std::forward<T>(begin), std::forward<T>(end), std::forward<T>(step));
}

}
