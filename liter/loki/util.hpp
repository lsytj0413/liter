// @file util.hpp
// @brief 工具类型
// @author
// @version
// @date

#pragma once

#include <type_traits>


namespace liter
{
namespace loki
{

// @class NullType
// @brief 空类型类, 一般用作占位符
class NullType {};

// @struct Int2Type
// @brief 将整数转换为类型
// @c++11 使用std::integral_constant
template <int v>
struct Int2Type : public std::integral_constant<int, v>
{};


// @struct Type2Type
// @brief 将类型转换为类型
template <typename T>
struct Type2Type
{
    using OriginalType = T;
};

}
}
