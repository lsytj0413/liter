// @file aspect.hpp
// @brief AOP(Aspect-Oriented Programming)简易实现
// @author
// @version
// @date

#pragma once

#include <utility>
#include <type_traits>

#include <liter/uncopyable.h>


namespace liter
{

// @macro HAS_MEMBER
// @brief 生成一个struct用于检查某个类型是否具有某个函数
// @param member: 函数名
#define HAS_MEMBER(member) \
    template <typename T, typename... TArgs> \
    struct has_member_##member \
    { \
    private: \
        template <typename U> \
        static auto Check(int) -> decltype(std::declval<U>().member(std::declval<TArgs>()...), std::true_type()); \
        template <typename U> \
        static std::false_type Check(...); \
    public: \
        enum { value = std::is_same<decltype(Check<T>(0)), std::true_type>::value }; \
    };

// @brief 定义 struct has_member_before
HAS_MEMBER(before)
// @brief 定义 struct has_member_after
HAS_MEMBER(after)


// @struct aspect
// @brief 一个函数的切面组织
template <typename Func, typename... TArgs>
struct aspect : uncopyable
{
private:
    Func m_func;

public:
    aspect(Func&& f) : m_func(std::forward<Func>(f)){};

    // @function
    // @brief 调用植入的函数
    // @param args...: 植入函数所需的参数包
    // @param aspect: 切面对象, 包含before和after函数
    // @return
    template <typename T>
    typename std::enable_if<has_member_before<T, TArgs...>::value && has_member_after<T, TArgs...>::value>::type
    invoke(TArgs&&... args, T&& aspect){
        aspect.before(std::forward<TArgs>(args)...);
        m_func(std::forward<TArgs>(args)...);
        aspect.after(std::forward<TArgs>(args)...);
    };

    // @function
    // @brief 调用植入的函数
    // @param args...: 植入函数所需的参数包
    // @param aspect: 切面对象, 包含before函数
    // @return
    template <typename T>
    typename std::enable_if<has_member_before<T, TArgs...>::value && !has_member_after<T, TArgs...>::value>::type
    invoke(TArgs&&... args, T&& aspect){
        aspect.before(std::forward<TArgs>(args)...);
        m_func(std::forward<TArgs>(args)...);
    };

    // @function
    // @brief 调用植入的函数
    // @param args...: 植入函数所需的参数包
    // @param aspect: 切面对象, 包含after函数
    // @return
    template <typename T>
    typename std::enable_if<!has_member_before<T, TArgs...>::value && has_member_after<T, TArgs...>::value>::type
    invoke(TArgs&&... args, T&& aspect){
        m_func(std::forward<TArgs>(args)...);
        aspect.after(std::forward<TArgs>(args)...);
    };

    // @function
    // @brief 调用植入的函数
    // @param args...: 植入函数所需的参数包
    // @param headAspect: 当前切面对象, 包含before和after函数
    // @param tailAspect...: 剩余切面对象
    // @return
    template <typename Head, typename... Tail>
    typename std::enable_if<has_member_before<Head, TArgs...>::value && has_member_after<Head, TArgs...>::value>::type
    invoke(TArgs&&... args, Head&& headAspect, Tail&&... tailAspect){
        headAspect.before(std::forward<TArgs>(args)...);
        invoke(std::forward<TArgs>(args)..., std::forward<Tail>(tailAspect)...);
        headAspect.after(std::forward<TArgs>(args)...);
    };

    // @function
    // @brief 调用植入的函数
    // @param args...: 植入函数所需的参数包
    // @param headAspect: 当前切面对象, 包含before函数
    // @param tailAspect...: 剩余切面对象
    // @return
    template <typename Head, typename... Tail>
    typename std::enable_if<has_member_before<Head, TArgs...>::value && !has_member_after<Head, TArgs...>::value>::type
    invoke(TArgs&&... args, Head&& headAspect, Tail&&... tailAspect){
        headAspect.before(std::forward<TArgs>(args)...);
        invoke(std::forward<TArgs>(args)..., std::forward<Tail>(tailAspect)...);
    };

    // @function
    // @brief 调用植入的函数
    // @param args...: 植入函数所需的参数包
    // @param headAspect: 当前切面对象, 包含after函数
    // @param tailAspect...: 剩余切面对象
    // @return
    template <typename Head, typename... Tail>
    typename std::enable_if<!has_member_before<Head, TArgs...>::value && has_member_after<Head, TArgs...>::value>::type
    invoke(TArgs&&... args, Head&& headAspect, Tail&&... tailAspect){
        invoke(std::forward<TArgs>(args)..., std::forward<Tail>(tailAspect)...);
        headAspect.after(std::forward<TArgs>(args)...);
    };
};

template <typename T>
using identity_t = T;

// @function
// @brief 切面植入调用
// @param fn: 被植入的函数
// @param args...: 植入函数所需的参数包
// @return
template <typename... AP, typename... TArgs, typename Func>
void invoke(Func&& fn, TArgs&&... args){
    aspect<Func, TArgs...> asp(std::forward<Func>(fn));
    asp.invoke(std::forward<TArgs>(args)..., identity_t<AP>()...);
};

}
