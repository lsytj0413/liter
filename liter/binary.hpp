// @file binary.hpp
// @brief 编译时2进制转10进制工具
// @author
// @version
// @date

#pragma once

#include <type_traits>


namespace liter
{

namespace detail
{

template <unsigned long long N>
struct binary_bit;

template <>
struct binary_bit<0>
{
    static long long const value = 0;
};

template <>
struct binary_bit<1>
{
    static long long const value = 1;
};

}


// @struct binary
// @brief 在编译时将2进制串转换为10进制
template <unsigned long long N>
struct binary
{
private:
		static long long const value_l = N / 10;
		static long long const value_r = N % 10;

		using binary_l_type = typename std::conditional < value_l < 10,
                                                                detail::binary_bit<value_l>,
                                                                binary < value_l >> ::type;
		using binary_r_type = typename detail::binary_bit<value_r>;

public:
		static long long const value = binary_l_type::value * 2 +
                                                      binary_r_type::value;
};

// @function
// @brief在编译时将2进制串转换为10进制
// @return long long
template <unsigned long long N>
long long binary_to()
{
    return binary<N>::value;
}

// @struct
// @brief 编译时获取2的N次方
template <unsigned int N>
struct shift_bit
{
public:
    static_assert( N<64, "N must less than 64." );
		static long long const value = 0x01 << N;
};

}


