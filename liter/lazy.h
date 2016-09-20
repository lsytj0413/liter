#pragma once

#include <functional>
using std::function;

#include <liter/optional.hpp>


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

    T value()
    {
        if (!init_p())
        {
            m_value = m_func();
        }

        return *m_value;
    }

    bool init_p() const
    {
        return m_value.init_p();
    }

private:
    std::function<T()> m_func;
    optional<T> m_value;
};

template <typename Func, typename... TArgs>
Lazy<typename std::result_of<Func(TArgs...)>::type> lazy(Func&& fn, TArgs&&... args)
{
    return Lazy<typename std::result_of<Func(TArgs...)>::type>(std::forward<Func>(fn),
                                                               std::forward<TArgs>(args)...);
}

}
