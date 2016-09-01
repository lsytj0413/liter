#pragma once

#include <memory>
#include <typeindex>
using std::unique_ptr;

namespace liter
{

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

    template <typename U, class = typename std::enable_if<!std::is_same<typename std::decay<U>::type,
                                                                        any>::value,
                                                          U>::type>
    any(U&& v): m_ptr(new Derived<typename std::decay<U>::type>(std::forward<U>(v))),
                m_type_index(std::type_index(typeid(typename std::decay<U>::type)))
    {};

    bool null_p() const
    {
        return !bool(m_ptr);
    };

    template <class U>
    bool is() const
    {
        return m_type_index == std::type_index(typeid(U));
    };

    template <class U>
    U& cast()
    {
        if(!is<U>())
        {
            throw std::logic_error{"bad case"};

        }

        auto derived = dynamic_cast<Derived<U>*>(m_ptr.get());
        return derived->m_value;
    };

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

    struct Base
    {
        virtual ~Base(){};
        virtual BasePtr clone() const = 0;
    };

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
