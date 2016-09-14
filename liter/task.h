#pragma once

#include <functional>
using std::function;


namespace liter
{

template <typename... R>
class task;

template <typename R, typename... TArgs>
class task<R(TArgs...)>
{
public:
    task(function<R(TArgs...)>&& f): m_fn(std::move(f)){};
    task(function<R(TArgs...)>& f): m_fn(f){};

    R run(TArgs&&... args)
    {
        return m_fn(std::forward<TArgs>(args)...);
    }

    template <typename F>
    auto then(F&& f) -> task<typename std::result_of<F(R)>::type(TArgs...)>
    {
        using result_type = typename std::result_of<F(R)>::type;

        auto func = std::move(m_fn);
        return task<result_type(TArgs...)>([func, &f](TArgs&&... args){
                return f(func(std::forward<TArgs>(args)...));
            });
    }

private:
    function<R(TArgs...)> m_fn;
};

}
