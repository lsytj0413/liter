#pragma once

#include <utility>
#include <type_traits>

#include <liter/uncopyable.h>


namespace liter
{

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

HAS_MEMBER(before)
HAS_MEMBER(after)

template <typename Func, typename... TArgs>
struct aspect : uncopyable
{
private:
    Func m_func;

public:
    aspect(Func&& f) : m_func(std::forward<Func>(f)){};

    template <typename T>
    typename std::enable_if<has_member_before<T, TArgs...>::value && has_member_after<T, TArgs...>::value>::type
    invoke(TArgs&&... args, T&& aspect){
        aspect.before(std::forward<TArgs>(args)...);
        m_func(std::forward<TArgs>(args)...);
        aspect.after(std::forward<TArgs>(args)...);
    };

    template <typename T>
    typename std::enable_if<has_member_before<T, TArgs...>::value && !has_member_after<T, TArgs...>::value>::type
    invoke(TArgs&&... args, T&& aspect){
        aspect.before(std::forward<TArgs>(args)...);
        m_func(std::forward<TArgs>(args)...);
    };

    template <typename T>
    typename std::enable_if<!has_member_before<T, TArgs...>::value && has_member_after<T, TArgs...>::value>::type
    invoke(TArgs&&... args, T&& aspect){
        m_func(std::forward<TArgs>(args)...);
        aspect.after(std::forward<TArgs>(args)...);
    };

    template <typename Head, typename... Tail>
    void invoke(TArgs&&... args, Head&& headAspect, Tail&&... tailAspect){
        headAspect.before(std::forward<TArgs>(args)...);
        invoke(std::forward<TArgs>(args)..., std::forward<Tail>(tailAspect)...);
        headAspect.after(std::forward<TArgs>(args)...);
    };
};

template <typename T>
using identity_t = T;

template <typename... AP, typename... TArgs, typename Func>
void invoke(Func&& fn, TArgs&&... args){
    aspect<Func, TArgs...> asp(std::forward<Func>(fn));
    asp.invoke(std::forward<TArgs>(args)..., identity_t<AP>()...);
};

}
