// @file HierarchyGenerator.hpp
// @brief 类自动生成模板
// @author
// @version
// @date

#pragma once

#include <liter/loki/typelist.hpp>
#include <liter/loki/util.hpp>


namespace liter
{
namespace loki
{


// @class GenScatterHierarchy
// @brief 散乱继承模板
template <typename T, template <class> class Unit>
class GenScatterHierarchy;

namespace detail
{

template <typename T, template <class> class Unit>
class ScatterHierarchySign;

template <typename Head, typename... TArgs, template <class> class Unit>
class ScatterHierarchySign<Typelist<Head, TArgs...>, Unit>
        : public GenScatterHierarchy<Head, Unit>
{};

}

template <typename T, template <class> class Unit>
class GenScatterHierarchy : public Unit<T>
{
public:
    using LeftBase = Unit<T>;
};

template <template <class> class Unit>
class GenScatterHierarchy<Typelist<>, Unit>
{};

template <typename Head, typename... TArgs, template <class> class Unit>
class GenScatterHierarchy<Typelist<Head, TArgs...>, Unit>
        : public detail::ScatterHierarchySign<Typelist<Head, TArgs...>, Unit>
        , public GenScatterHierarchy<Typelist<TArgs...>, Unit>
{
public:
    using LeftBase = detail::ScatterHierarchySign<Typelist<Head, TArgs...>, Unit>;
    using RightBase = GenScatterHierarchy<Typelist<TArgs...>, Unit>;
};


namespace detail
{

template <template <class> class Unit, typename Head, typename... TArgs>
Unit<Head>& FieldHelper(GenScatterHierarchy<Typelist<Head, TArgs...>, Unit>& obj,
                        const Int2Type<0>)
{
    typename GenScatterHierarchy<Typelist<Head, TArgs...>, Unit>::LeftBase& leftBase = obj;
    return leftBase;
};

template <int i, template <class> class Unit, typename Head, typename... TArgs>
Unit<typename TypeAt<Typelist<Head, TArgs...>, i>::Result>&
FieldHelper(GenScatterHierarchy<Typelist<Head, TArgs...>, Unit>& obj,
                        const Int2Type<i>)
{
    GenScatterHierarchy<Typelist<TArgs...>, Unit>& rightBase = obj;
    // typename GenScatterHierarchy<Typelist<Head, TArgs...>, Unit>::RightBase& rightBase = obj;
    return FieldHelper(rightBase, Int2Type<i-1>());
};

}


// @function
// @brief 将散乱生成的类转换为基类类型(按索引)
// @param obj: 生成类
// @return Unit<typename TypeAt<Typelist<TArgs...>, i>::Result>的引用
template <int i, class... TArgs, template <class> class Unit>
Unit<typename TypeAt<Typelist<TArgs...>, i>::Result>&
Field(GenScatterHierarchy<Typelist<TArgs...>, Unit>& obj)
{
    return detail::FieldHelper(obj, Int2Type<i>());
};

template <int i, class... TArgs, template <class> class Unit>
const Unit<typename TypeAt<Typelist<TArgs...>, i>::Result>&
Field(const GenScatterHierarchy<Typelist<TArgs...>, Unit>& obj)
{
    return detail::FieldHelper(
        const_cast<GenScatterHierarchy<Typelist<TArgs...>, Unit>&>(obj),
        Int2Type<i>());
};


// @function
// @brief 将散乱生成的类转换为基类类型(按类型)
// @param obj: 生成类
// @return Unit<T>的引用
template <class T, class... TArgs, template <class> class Unit>
Unit<T>& Field(GenScatterHierarchy<Typelist<TArgs...>, Unit>& obj)
{
    return Field<
        IndexOf<Typelist<TArgs...>, T>::value
        >(obj);
};

template <class T, class... TArgs, template <class> class Unit>
const Unit<T>& Field(const GenScatterHierarchy<Typelist<TArgs...>, Unit>& obj)
{
    return Field<
        IndexOf<Typelist<TArgs...>, T>::value
        >(obj);
};


namespace detail
{

template <class T>
struct TupleUnit
{
    T value_;
    operator T&() {
        return value_;
    };
    operator const T&() const {
        return value_;
    };
};

}


// @struct Tuple
// @brief tuple的实现, 可容纳多个不同类型的值的容器
// @c++11 使用std::tuple
template <typename TL>
struct Tuple : public GenScatterHierarchy<TL, detail::TupleUnit>
{};

template <typename... TArgs>
struct Tuple<Typelist<TArgs...>> : public GenScatterHierarchy<Typelist<TArgs...>, detail::TupleUnit>
{};


// @class GenLinearHierarchy
// @brief 生成线性继承类
template <typename T, template <class, class> class Unit>
class GenLinearHierarchy;

template <template <class, class> class Unit>
class GenLinearHierarchy<Typelist<>, Unit>
{};

template <typename Head, typename... TArgs, template <class, class> class Unit>
class GenLinearHierarchy<Typelist<Head, TArgs...>, Unit>
        : public Unit<Head, GenLinearHierarchy<Typelist<TArgs...>, Unit>>
{};


}
}
