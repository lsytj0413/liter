#pragma once

#include <cstddef>
#include <stdexcept>
using std::logic_error;


namespace liter
{

namespace detail
{

template <typename T>
class range_iterator
{
public:
    using value_type = T;
    using size_type = size_t;

    range_iterator(size_type cur_start, value_type begin_val, value_type step_val)
            : m_cursor(cur_start)
            , m_value(begin_val)
            , m_step(step_val)
    {
        m_value += (m_step * m_cursor);
    };

    value_type operator*() const
    {
        return m_value;
    };

    bool operator!=(const range_iterator& rhs) const
    {
        return (m_cursor != rhs.m_cursor);
    };

    range_iterator& operator++(void)
    {
        m_value += m_step;
        ++ m_cursor;
        return (*this);
    };

private:
    size_type m_cursor;
    const value_type m_step;
    value_type m_value;

};

}

template <typename T>
class range_impl
{
public:
    using value_type = T;
    using reference = const value_type&;
    using const_reference = const value_type&;
    using iterator = const detail::range_iterator<value_type>;
    using const_iterator = const detail::range_iterator<value_type>;
    using size_type = typename detail::range_iterator<value_type>::iterator;

    range_impl(value_type begin_val, value_type end_val, value_type step_val)
            : m_begin(begin_val)
            , m_end(end_val)
            , m_step(step_val)
            , m_max_count(get_adjusted_count())
    {};

    size_type size() const
    {
        return m_max_count;
    };

    const_iterator begin() const
    {
        return {0, m_begin, m_end};
    };

    const_iterator end() const
    {
        return {m_max_count, m_begin, m_end};
    };

private:
    const value_type m_begin;
    const value_type m_end;
    const value_type m_step;
    const value_type m_max_count;

private:
    size_type get_adjusted_count() const
    {
        if (m_step > 0 && m_begin >= m_end)
        {
            throw logic_error("End value must be greator than begin value.");
        }
        else if(m_step < 0 && m_begin <= m_end)
        {
            throw logic_error("End value must be less than begin value.");
        }

        size_type x = static_cast<size_type>((m_end - m_begin) / m_step);
        if (m_begin + (m_step * x) != m_end)
        {
            ++x;
        }

        return x;
    }
};

template <typename T>
range_impl<T> range(T&& end)
{
    return {{}, std::forward(end), 1};
}

template <typename T>
range_impl<T> range(T&& begin, T&& end)
{
    return {std::forward(begin), std::forward(end), 1};
}

template <typename T, typename U>
auto range(T&& begin, T&& end, U&& step) -> range_impl<decltype(begin + step)>
{
    using return_type = range_impl<decltype(begin + step)>;
    return return_type(std::forward(begin), std::forward(end), std::forward(step));
}

}
