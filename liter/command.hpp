// @file command.hpp
// @brief 命令模式实现模板
// @author
// @version
// @date

#pragma once

#include <functional>


namespace liter
{

// @struct command
// @brief 命令模式包装类
template <typename R=void>
struct command
{
private:
    std::function<R()> m_fn;

public:
    // @function
    // @brief 包装函数到命令
    // @param f: 被包装函数
    // @param args: 可变参数列表
    // @return
    template <class F, class... TArgs,
              class = typename std::enable_if<!std::is_member_function_pointer<F>::value>::type>
    void wrap(F&& f, TArgs&&... args){
        m_fn = [&](){
            return f(args...);
        };
    };

    // @function
    // @brief 包装函数到命令, 适用于const成员函数
    // @param f: 被包装函数
    // @param p: 对象指针
    // @param args: 可变参数列表
    // @return
    template <class R1, class C, class... DArgs,
              class P, class... TArgs>
    void wrap(R1(C::*f)(DArgs...) const, P&& p, TArgs&&... args){
        m_fn = [=](){
            return (*p.*f)(args...);
        };
    };

    // @function
    // @brief 包装函数到命令, 适用于成员函数
    // @param f: 被包装函数
    // @param p: 对象指针
    // @param args: 可变参数列表
    // @return
    template <class R1, class C, class... DArgs,
              class P, class... TArgs>
    void wrap(R1(C::*f)(DArgs...), P&& p, TArgs&&... args){
        m_fn = [=](){
            return (*p.*f)(args...);
        };
    };

    // @function
    // @brief 执行命令
    // @return R
    R execute()
    {
        return m_fn();
    };
};

}
