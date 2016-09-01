# pragma once

#include <functional>


namespace liter
{

template <typename T>
struct function_traits;

template <typename Ret, typename... TArgs>
struct function_traits<Ret(TArgs...)>
{
public:
    enum {
        arity = sizeof...(TArgs)
    };

    using function_type = Ret(TArgs...);
    using return_type = Ret;

    using stl_function_type = std::function<function_type>;
    using pointer = Ret(*)(TArgs...);

    template <size_t N>
    using arg_type = typename std::tuple_element<N, std::tuple<TArgs...>>::type;

    template <size_t I>
    struct args
    {
        static_assert(I < arity, "index is out of range, must be less than sizeof args");
        using type = typename std::tuple_element<I, std::tuple<TArgs...>>::type;
    };
};

template <typename Ret, typename... TArgs>
struct function_traits<Ret(*)(TArgs...)> : public function_traits<Ret(TArgs...)>
{};

template <typename Ret, typename... TArgs>
struct function_traits<std::function<Ret(TArgs...)>> : public function_traits<Ret(TArgs...)>
{};

#define FUNCTION_TRAITS(...) \
    template <typename ReturnType, typename ClassType, typename... TArgs>\
    struct function_traits<ReturnType(ClassType::*)(TArgs...) __VA_ARGS__> : function_traits<ReturnType(TArgs...)>{}; \

FUNCTION_TRAITS()
FUNCTION_TRAITS(const)
FUNCTION_TRAITS(volatile)
FUNCTION_TRAITS(const volatile)

        ;
template <typename Callable>
struct function_traits : function_traits<decltype(&Callable::operator())>
{};

template <typename Function>
typename function_traits<Function>::stl_function_type
to_function(const Function& lambda)
{
    return static_cast<typename function_traits<Function>::stl_function_type>(lambda);
}

template <typename Function>
typename function_traits<Function>::stl_function_type
to_function(const Function&& lambda)
{
    return static_cast<typename function_traits<Function>::stl_function_type>(
        std::forward<Function>(lambda));
}

template <typename Function>
typename function_traits<Function>::pointer
to_function_pointer(const Function& lambda)
{
    return static_cast<typename function_traits<Function>::pointer>(lambda);
}

}
