// @file optional.hpp
// @brief 可选值模板
// @author
// @version
// @date

#pragma once

#include <type_traits>
#include <utility>
#include <stdexcept>


namespace liter
{

// @class optional
// @brief 可选类
// @c++17 可使用std::optional
template <typename T,
          typename = typename std::enable_if<!std::is_same<void, T>::value>::type
          >
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

    optional(const T& v) {
        create(v);
    }

    optional(const optional& rhs) {
        if (rhs.init_p())
        {
            assign(rhs);
        }
    }

    ~optional() {
        destroy();
    }

    // @function
    // @brief 构造
    // @param args: 可变参数列表
    // @return
    template <typename... TArgs>
    void emplace(TArgs&&... args) {
        destroy();
        create(std::forward<TArgs>(args)...);
    }

    // @function
    // @brief 是否已经初始化
    // return bool
    bool init_p() const {
        return m_init;
    }

    // @function
    // @brief 获取值
    // @return T
    T const& operator*() const {
        if (init_p()) {
            return *((T*)(&m_data));
        }

        throw std::logic_error("optional hasn't init.");
    }

private:
    template <typename... TArgs>
    void create(TArgs&&... args) {
        new (&m_data) T(std::forward<TArgs>(args)...);
        m_init = true;
    }

    void destroy() {
        if (init_p()) {
            m_init = false;
            ((T*)(&m_data))->~T();
        }
    }

    void assign(const optional& rhs) {
        if (rhs.init_p()) {
            copy(rhs.m_data);
            m_init = true;
        }
        else {
            destroy();
        }
    }

    void copy(const data_t& val) {
        destroy();
        new (&m_data) T(*((T*)(&val)));
    }
};

}
