#pragma once

#include <functional>


namespace liter
{

template <typename R=void>
struct command
{
private:
    std::function<R()> m_fn;

public:
    template <class F, class... TArgs,
              class = typename std::enable_if<!std::is_member_function_pointer<F>::value>::type>
    void wrap(F&& f, TArgs&&... args){
        m_fn = [&](){
            return f(args...);
        };
    };

    template <class R1, class C, class... DArgs,
              class P, class... TArgs>
    void wrap(R1(C::*f)(DArgs...) const, P&& p, TArgs&&... args){
        m_fn = [&, f](){
            return (*p.*f)(args...);
        };
    };

    template <class R1, class C, class... DArgs,
              class P, class... TArgs>
    void wrap(R1(C::*f)(DArgs...), P&& p, TArgs&&... args){
        m_fn = [&, f](){
            return (*p.*f)(args...);
        };
    };

    R execute()
    {
        return m_fn();
    };
};

}
