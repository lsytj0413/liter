#pragma once

#include <type_traits>
#include <utility>
#include <stdexcept>


namespace liter
{

template <typename T>
class optional
{
private:
    using data_t = typename std::aligned_storage<sizeof(T),
                                                 std::alignment_of<T>::value>::type;
private:
    bool m_init = false;    // 是否已经初始化
    data_t m_data;          // 内存对齐的缓冲区

public:
    optional(){};

    optional(const T& v)
    {
        create(v);
    }

    optional(const optional& rhs)
    {
        if (rhs.is_init())
        {
            assign(rhs);
        }
    }

    ~optional()
    {
        destroy();
    }

    template <typename... TArgs>
    void emplace(TArgs&&... args)
    {
        destroy();
        create(std::forward<TArgs>(args)...);
    }

    bool is_init() const
    {
        return m_init;
    }

    T const& operator*() const
    {
        if (is_init())
        {
            return *((T*)(&m_data));
        }

        throw std::logic_error("optional hasn't init.");
    }

private:
    template <typename... TArgs>
    void create(TArgs&&... args)
    {
        new (&m_data) T(std::forward<TArgs>(args)...);
        m_init = true;
    }

    void destroy()
    {
        if (is_init())
        {
            m_init = false;
            ((T*)(&m_data))->~T();
        }
    }

    void assign(const optional& rhs)
    {
        if (rhs.is_init())
        {
            copy(rhs.m_data);
            m_init = true;
        }
        else
        {
            destroy();
        }
    }

    void copy(const data_t& val)
    {
        destroy();
        new (&m_data) T(*((T*)(&val)));
    }
};

}
