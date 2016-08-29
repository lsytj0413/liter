#pragma once

#include <functional>
using std::function;

#include "liter/utils/optional.h"


namespace liter
{

template <typename T>
struct Lazy
{
    Lazy(){};

    template <typename Func, typename... TArgs>
    Lazy(Func&& f, TArgs&& ...args)
    {
        m_func = [&f, &args...]{
            return f(args...);
        };
    }

    T& value()
    {
        if (!m_func.is_init())
        {
            m_value = m_func();
        }

        return *m_value;
    }

    bool init() const
    {
        return m_value.is_init();
    }

private:
    std::function<T()> m_func;
    optional<T> m_value;
};

template <typename Func, typename... TArgs>
Lazy<typename std::result_of<Func(TArgs...)>::type> lazy(Func&& fun, TArgs&&... args)
{
    return Lazy<typename std::result_of<Func(TArgs...)>::type>(std::forward<Func>(fun),
                                                               std::forward<TArgs>(args)...);
}

}
