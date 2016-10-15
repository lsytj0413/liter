// @file function_traits.hpp
// @brief 函数特征提取
// @author
// @version
// @date

#pragma once

#include <functional>


namespace liter
{

template <typename T>
struct function_traits;

// @struct function_traits
// @brief 普通函数特征提取
template <typename Ret, typename... TArgs>
struct function_traits<Ret(TArgs...)>
{
public:
    // @enum
    // brief 函数参数个数
    enum {
        arity = sizeof...(TArgs)
    };

    // @brief 函数类型
    using function_type = Ret(TArgs...);
    // @brief 函数指针类型
    using pointer = Ret(*)(TArgs...);
    // brief 返回值类型
    using return_type = Ret;

    // @brief std::function类型
    using stl_function_type = std::function<function_type>;

    // brief 参数类型
    template <size_t N>
    using arg_type = typename std::tuple_element<N, std::tuple<TArgs...>>::type;

    // brief 参数类型
    template <size_t I>
    struct args
    {
        static_assert(I < arity, "index is out of range, must be less than sizeof args");
        using type = typename std::tuple_element<I, std::tuple<TArgs...>>::type;
    };
};

// @struct function_traits
// @brief 函数指针特征提取
template <typename Ret, typename... TArgs>
struct function_traits<Ret(*)(TArgs...)> : public function_traits<Ret(TArgs...)>
{};

// @struct function_traits
// @brief std::funtion特征提取
template <typename Ret, typename... TArgs>
struct function_traits<std::function<Ret(TArgs...)>> : public function_traits<Ret(TArgs...)>
{};

#define FUNCTION_TRAITS(...) \
    template <typename ReturnType, typename ClassType, typename... TArgs>\
    struct function_traits<ReturnType(ClassType::*)(TArgs...) __VA_ARGS__> : function_traits<ReturnType(TArgs...)>{};

FUNCTION_TRAITS()
FUNCTION_TRAITS(const)
FUNCTION_TRAITS(volatile)
FUNCTION_TRAITS(const volatile)


// @struct function_traits
// @brief 仿函数提取
template <typename Callable>
struct function_traits : function_traits<decltype(&Callable::operator())>
{};

// @function
// @brief lambda转换为std::function
// @param lambda: lambda表达式
// @return std::function
template <typename Function>
auto to_function(const Function& lambda)
{
    return static_cast<typename function_traits<Function>::stl_function_type>(lambda);
}

// @function
// @brief lambda转换为std::function
// @param lambda: lambda表达式
// @return std::function
template <typename Function>
auto to_function(const Function&& lambda)
{
    return static_cast<typename function_traits<Function>::stl_function_type>(
        std::forward<Function>(lambda));
}

// @function
// @brief lambda转换为函数指针
// @param lambda: lambda表达式
// @return 函数指针
template <typename Function>
auto to_function_pointer(const Function& lambda)
{
    return static_cast<typename function_traits<Function>::pointer>(lambda);
}

}
