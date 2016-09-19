#pragma once

#include <typeindex>
#include <type_traits>
#include <utility>

#include <liter/function_traits.hpp>


namespace liter
{

template <typename T, typename U>
struct convertiale
{
private:
    static std::true_type Check(T);
    static std::false_type Check(...);

public:
    enum {
        value = std::is_same<decltype(Check(std::declval<U>())), std::true_type>::value
    };
};

template <std::size_t arg, std::size_t... rest>
struct IntegerMax;

template <std::size_t arg>
struct IntegerMax<arg> : public std::integral_constant<std::size_t, arg>
{};

template <std::size_t arg1, std::size_t arg2, std::size_t... rest>
struct IntegerMax<arg1, arg2, rest...> :
            public std::integral_constant<std::size_t,
                                          arg1>=arg2 ? IntegerMax<arg1, rest...>::value : IntegerMax<arg2, rest...>::value>
{};

template <typename... TArgs>
struct MaxAlign : public std::integral_constant<int, IntegerMax<std::alignment_of<TArgs>::value...>::value>
{};

template <typename T, typename... List>
struct Contains;

template <typename T, typename Head, typename... Rest>
struct Contains<T, Head, Rest...> :
            public std::conditional<std::is_same<T, Head>::value,
                                    std::true_type,
                                    Contains<T, Rest...>>::type
{};

template <typename T>
struct Contains<T> : public std::false_type
{};

template <typename T, typename... List>
struct IndexOf;

template <typename T, typename Head, typename... Rest>
struct IndexOf<T, Head, Rest...>
{
    enum {
        value = IndexOf<T, Rest...>::value + 1
    };
};

template <typename T, typename... Rest>
struct IndexOf<T, T, Rest...>
{
    enum {
        value = 0
    };
};

template <typename T>
struct IndexOf<T>
{
    enum {
        value = -1
    };
};

template <int index, typename... Types>
struct At;

template <int index, typename First, typename... Types>
struct At<index, First, Types...>
{
    using type = typename At<index - 1, Types...>::type;
};

template <typename T, typename... Types>
struct At<0, T, Types...>
{
    using type = T;
};

template <typename... Types>
class variant
{
private:
    enum {
        data_size = IntegerMax<sizeof(Types)...>::value,
        align_size = MaxAlign<Types...>::value
    };

    using data_t = typename std::aligned_storage<data_size, align_size>::type;

public:
    template <int index>
    using IndexType = typename At<index, Types...>::type;

    variant(void): m_type_index(typeid(void))
    {};

    ~variant()
    {
        destroy();
    };

    variant(variant<Types...>&& old) : m_type_index(old.m_type_index)
    {
        move(old.m_type_index, &old.m_data, &m_data);
    };

    variant(variant<Types...>& old) : m_type_index(old.m_type_index)
    {
        copy(old.m_type_index, &old.m_data, &m_data);
    };

    variant& operator= (const variant& old)
    {
        copy(old.m_type_index, &old.m_data, &m_data);
        m_type_index = old.m_type_index;
        return *this;
    };

    variant& operator= (const variant&& old)
    {
        move(old.m_type_index, &old.m_data, &m_data);
        m_type_index = old.m_type_index;
        return *this;
    };

    template <class T,
              class = typename std::enable_if<Contains<typename std::decay<T>::type, Types...>::value>::type>
    variant(T&& value) : m_type_index(typeid(void))
    {
        destroy();
        using U = typename std::decay<T>::type;
        new(&m_data) U(std::forward<T>(value));
        m_type_index = std::type_index(typeid(U));
    };

    template <typename T>
    bool is() const
    {
        return m_type_index == std::type_index(typeid(T));
    };

    bool empty() const
    {
        return m_type_index == std::type_index(typeid(void));
    };

    std::type_index type() const
    {
        return m_type_index;
    };

    template <typename T>
    typename std::decay<T>::type& get()
    {
        using U = typename std::decay<T>::type;
        if (!is<U>())
        {
            throw std::bad_cast();
        }

        return *(U*)(&m_data);
    };

    template <typename T>
    int index()
    {
        return IndexOf<T, Types...>::value;
    };

    template <typename F>
    void visit(F&& f){
        using T = typename liter::function_traits<F>::template args<0>::type;

        if (is<T>()){
            f(get<T>());
        }
    };

    template <typename F, typename... Rest>
    void visit(F&& f, Rest&&... rest){
        using T = typename liter::function_traits<F>::template args<0>::type;

        if (is<T>()){
            f(get<T>());
        }
        else {
            visit(std::forward<Rest>(rest)...);
        }
    };

    bool operator== (const variant& rhs) const
    {
        return m_type_index == rhs.m_type_index;
    };

    bool operator< (const variant& rhs) const
    {
        return m_type_index < rhs.m_type_index;
    };
private:
    void destroy(const std::type_index& index, void *buf)
    {
        [](Types&&...){}((destroy0<Types>(index, buf), 0)...);
    };

    template <typename T>
    void destroy0(const std::type_index& id, void* data)
    {
        if (id == std::type_index(typeid(T)))
        {
            reinterpret_cast<T*>(data)->~T();
        }
    };

    void move(const std::type_index& old_t, void *old_v, void *new_v)
    {
        [](Types&&...){}((move0<Types>(old_t, old_v, new_v), 0)...);
    };

    template <typename T>
    void move0(const std::type_index& old_t, void* old_v, void* new_v)
    {
        if (old_t == std::type_index(typeid(T)))
        {
            new (new_v)T(std::move(*reinterpret_cast<T*>(old_v)));
        }
    };

    void copy(const std::type_index& old_t, void *old_v, void *new_v)
    {
        [](Types&&...){}((copy0<Types>(old_t, old_v, new_v), 0)...);
    };

    template <typename T>
    void copy0(const std::type_index& old_t, void* old_v, void* new_v)
    {
        if (old_t == std::type_index(typeid(T)))
        {
            new (new_v)T(*reinterpret_cast<T*>(old_v));
        }
    };

private:
    data_t m_data;
    std::type_index m_type_index;
};

}
