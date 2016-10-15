// @file lazy.h
// @brief 惰性求值
// @author
// @version
// @date

#pragma once

#include <functional>
using std::function;

#include <liter/optional.hpp>


namespace liter
{

// @struct Lazy
// @brief 惰性求值实现类
template <typename T>
struct Lazy
{
    Lazy(){};

    template <typename Func, typename... TArgs>
    Lazy(Func&& f, TArgs&& ...args) {
        m_func = [&f, &args...]{
            return f(args...);
        };
    }

    // @function
    // @brief 获取值
    // @return T
    T value() {
        if (!init_p())
        {
            m_value = m_func();
        }

        return *m_value;
    }

    // @function
    // @brief 是否已经求值
    // @return bool
    bool init_p() const {
        return m_value.init_p();
    }

private:
    std::function<T()> m_func;
    optional<T> m_value;
};

// @function
// @brief 辅助创建惰性求值类
// @param fn: 需惰性求值的函数
// @param args: 可变参数列表
// @return Lazy
template <typename Func, typename... TArgs>
Lazy<typename std::result_of<Func(TArgs...)>::type>
lazy(Func&& fn, TArgs&&... args) {
    return Lazy<typename std::result_of<Func(TArgs...)>::type>(std::forward<Func>(fn),
                                                               std::forward<TArgs>(args)...);
}

}
