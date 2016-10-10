// @file any.hpp
// @brief 可存放任意类型的any
// @author
// @version
// @date

#pragma once

#include <memory>
#include <typeindex>
using std::unique_ptr;


namespace liter
{

// @class any
// @brief 任意类型的容器
class any
{
public:
    any(void): m_type_index(std::type_index(typeid(void))){};
    any(const any& rhs): m_ptr(rhs.clone()),
                         m_type_index(rhs.m_type_index)
    {};
    any(any&& rhs): m_ptr(std::move(rhs.m_ptr)),
                    m_type_index(rhs.m_type_index)
    {};

    // @function
    // @brief 普通构造函数, 通过值构造
    // @param v: 值
    template <typename U, class = typename std::enable_if<!std::is_same<typename std::decay<U>::type,
                                                                        any>::value,
                                                          U>::type>
    any(U&& v): m_ptr(new Derived<typename std::decay<U>::type>(std::forward<U>(v))),
                m_type_index(std::type_index(typeid(typename std::decay<U>::type)))
    {};

    // @function
    // @brief 是否初始化
    // @return bool
    bool null_p() const
    {
        return !bool(m_ptr);
    };

    // @function
    // @brief 是否是某个类型的值
    // @return bool
    template <class U>
    bool is() const
    {
        return m_type_index == std::type_index(typeid(typename std::decay<U>::type));
    };

    // @function
    // @brief 获取值
    // @return U类型的对象
    template <class U>
    typename std::decay<U>::type& cast()
    {
        using U1 = typename std::decay<U>::type;
        if(!is<U1>())
        {
            throw std::logic_error{"bad case"};
        }

        auto derived = dynamic_cast<Derived<U1>*>(m_ptr.get());
        return derived->m_value;
    };

    // @function
    // @brief 赋值
    // @param rhs: any对象
    // @return any
    any& operator=(const any& rhs)
    {
        if (m_ptr == rhs.m_ptr)
        {
            return *this;
        }

        m_ptr = rhs.clone();
        m_type_index = rhs.m_type_index;
        return *this;
    };

private:
    struct Base;
    using BasePtr = unique_ptr<Base>;

    // @struct Base
    // @brief 基类, 用于类型擦除
    struct Base
    {
        virtual ~Base(){};
        virtual BasePtr clone() const = 0;
    };

    // @struct Derived
    // @brief 派生类, 保存具体的类型信息
    template <typename T>
    struct Derived : public Base
    {
        template <typename U>
        Derived(U&& v) : m_value(std::forward<U>(v)){};

        BasePtr clone() const
        {
            return BasePtr(new Derived<T>(m_value));
        }

        T m_value;
    };

    BasePtr clone() const
    {
        if (m_ptr != nullptr)
        {
            return m_ptr->clone();
        }

        return nullptr;
    }

    BasePtr m_ptr;
    std::type_index m_type_index;
};

}
