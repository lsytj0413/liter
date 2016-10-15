// @file variant.hpp
// @brief 可变值容器
// @author
// @version
// @date

#pragma once

#include <typeindex>
#include <type_traits>
#include <utility>

#include <liter/function_traits.hpp>


namespace liter
{

// @struct convertible
// @brief 类型隐式转换判定类
template <typename... TArgs>
struct convertible;

template <typename T, typename U>
struct convertible<T, U>
{
private:
    static std::true_type Check(T);
    static std::false_type Check(...);

public:
    enum {
        value = std::is_same<decltype(Check(std::declval<U>())), std::true_type>::value
    };
};

template <typename T>
struct convertible<T, T>
{
public:
    enum {
        value = true
    };
};

// @struct IntegerMax
// @brief 获取最大整型值
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

// @struct MaxAlign
// @brief 获取类型中的最大内存对齐值
template <typename... TArgs>
struct MaxAlign : public std::integral_constant<int, IntegerMax<std::alignment_of<TArgs>::value...>::value>
{};

// @struct Contains
// @brief 类型是否在类型列表中
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

// @struct ConvContains
// @brief 类型是否可隐式转换为类型列表中的类型
template <typename T, typename... List>
struct ConvContains;

template <typename T, typename Head, typename... Rest>
struct ConvContains<T, Head, Rest...> :
            public std::conditional<convertible<Head, T>::value,
                                    std::true_type,
                                    ConvContains<T, Rest...>>::type
{};

template <typename T>
struct ConvContains<T> : public std::false_type
{};

// @struct ConvType
// @brief 可隐式转换为的类型
template <typename... T>
struct ConvType;

template <typename T, typename Head, typename... Rest>
struct ConvType<T, Head, Rest...>
{
    using type = typename std::conditional<convertible<Head, T>::value,
                                           Head,
                                           ConvType<T, Rest...>>::type;
};

template <typename T>
struct ConvType<T>{};

// @struct IndexOf
// @brief 类型在类型列表中的索引
template <typename T, typename... List>
struct IndexOf;

template <typename T, typename Head, typename... Rest>
struct IndexOf<T, Head, Rest...>
{
    enum {
        value = IndexOf<T, Rest...>::value == -1 ? -1 : IndexOf<T, Rest...>::value + 1
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

// @struct At
// @brief 在类型列表中对应索引的类型
template <size_t index, typename... Types>
struct At;

template <size_t index, typename First, typename... Types>
struct At<index, First, Types...>
{
    using type = typename At<index - 1, Types...>::type;
};

template <typename T, typename... Types>
struct At<0, T, Types...>
{
    using type = T;
};

// @class variant
// @brief 可变类型容器
template <typename... Types>
class variant
{
private:
    enum {
        data_size = IntegerMax<sizeof(Types)...>::value,        // 最大类型尺寸
        align_size = MaxAlign<Types...>::value                  // 最大类型内存对齐值
    };

    using data_t = typename std::aligned_storage<data_size, align_size>::type;

private:
    data_t m_data;
    std::type_index m_type_index;

private:

    // @struct DataEqual
    // @brief 值是否相等
    template <typename... TArgs>
    struct DataEqual{
        static bool equal(const std::type_index& index, void* old_v, void* new_v)
        {
            return false;
        };
    };

    template <typename T, typename... Rest>
    struct DataEqual<T, Rest...>
    {
        static bool equal(const std::type_index& index, void* old_v, void* new_v)
        {
            if (index == std::type_index(typeid(T))){
                bool b = (*reinterpret_cast<T*>(old_v)) == (*reinterpret_cast<T*>(new_v));
                if (b){
                    return true;
                }
            }

            return DataEqual<Rest...>::equal(index, old_v, new_v);
        }
    };

    // @function
    // @brief 销毁值
    // @param index: std::type_index
    // @param buf: 待销毁值
    // @return
    void destroy(const std::type_index& index, void *buf)
    {
        [](...){}((destroy0<Types>(index, buf), 0)...);
    };

    template <typename T>
    void destroy0(const std::type_index& id, void* data)
    {
        if (id == std::type_index(typeid(T))) {
            reinterpret_cast<T*>(data)->~T();
        }
    };

    void move(const std::type_index& old_t, void *old_v, void *new_v)
    {
        [](...){}((move0<Types>(old_t, old_v, new_v), 0)...);
    };

    template <typename T>
    void move0(const std::type_index& old_t, void* old_v, void* new_v)
    {
        if (old_t == std::type_index(typeid(T))) {
            new (new_v)T(std::move(*reinterpret_cast<T*>(old_v)));
        }
    };

    void copy(const std::type_index& old_t, void *old_v, void *new_v)
    {
        [](...){}((copy0<Types>(old_t, old_v, new_v), 0)...);
    };

    template <typename T>
    void copy0(const std::type_index& old_t, void* old_v, void* new_v)
    {
        if (old_t == std::type_index(typeid(T)))
        {
            new (new_v)T(*reinterpret_cast<T*>(old_v));
        }
    };

    template <typename T> void cons(T&& value, std::true_type){
        // 当有相同的类型时调用
        destroy(m_type_index, &m_data);
        using U = typename std::decay<T>::type;
        new(&m_data) U(std::forward<T>(value));
        m_type_index = std::type_index(typeid(U));
    }

    template <typename T>
    void cons(T&& value, std::false_type){
        // 当没相同的类型，但是可以隐式转换时调用
        destroy(m_type_index, &m_data);
        using U = typename ConvType<typename std::decay<T>::type, Types...>::type;
        new(&m_data) U(std::forward<T>(value));
        m_type_index = std::type_index(typeid(U));
    }

public:
    template <int index>
    using IndexType = typename At<index, Types...>::type;

    variant(void): m_type_index(typeid(void))
    {};

    ~variant()
    {
        destroy(m_type_index, &m_data);
    };

    variant(variant<Types...>&& old) : m_type_index(old.m_type_index)
    {
        move(old.m_type_index, &old.m_data, &m_data);
    };

    variant(const variant<Types...>& old) : m_type_index(old.m_type_index)
    {
        copy(old.m_type_index, const_cast<data_t*>(&old.m_data), &m_data);
    };

    variant& operator= (const variant& old)
    {
        copy(old.m_type_index, const_cast<data_t*>(&old.m_data), &m_data);
        m_type_index = old.m_type_index;
        return *this;
    };

    variant& operator= (variant&& old)
    {
        move(old.m_type_index, &old.m_data, &m_data);
        m_type_index = old.m_type_index;
        return *this;
    };

    template <class T,
              class = typename std::enable_if<ConvContains<typename std::decay<T>::type, Types...>::value>::type>
    variant(T&& value) : m_type_index(typeid(void))
    {
        cons<T>(std::forward<T>(value), Contains<typename std::decay<T>::type, Types...>());
    };

    // @function
    // @brief 是否存储某个类型的值
    // @return bool
    template <typename T>
    bool is() const {
        return m_type_index == std::type_index(typeid(T));
    };

    // @function
    // @brief 是否未存储值
    // @return bool
    bool empty() const {
        return m_type_index == std::type_index(typeid(void));
    };

    // @function
    // @brief 获取当前存储类型的type_index
    // @return std::type_index
    std::type_index type() const {
        return m_type_index;
    };

    // @function
    // @brief 获取值
    // @return T
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

    // @function
    // @brief 获取类型索引
    // @return int
    template <typename T>
    int index() {
        return IndexOf<T, Types...>::value;
    };

    // @function
    // @brief 访问存储的值
    // @param f: 访问函数
    // @return
    template <typename F>
    void visit(F&& f){
        using T = typename liter::function_traits<F>::template args<0>::type;

        if (is<T>()){
            f(get<T>());
        }
    };

    // @function
    // @brief 访问存储的值
    // @param f: 访问函数
    // @param rest: 访问函数列表
    // @return
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
        if (m_type_index == rhs.m_type_index){
            if (m_type_index == std::type_index(typeid(void))){
                return true;
            }

            return DataEqual<Types...>::equal(rhs.m_type_index, const_cast<data_t*>(&rhs.m_data), const_cast<data_t*>(&m_data));
        }

        return false;
    };

    bool operator<(const variant& rhs) const
    {
        return m_type_index < rhs.m_type_index;
    }
};

}
