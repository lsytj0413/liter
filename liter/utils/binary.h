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


// 将二进制转换为十进制
// 例如: 5 = binary<101>
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

template <unsigned long long N>
long long binary_to()
{
    return binary<N>::value;
}

template <unsigned int N>
struct shift_bit
{
public:
		static long long const value = 0x01 << N;
};

}


