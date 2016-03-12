#pragma once

namespace lt
{
	template <typename T, typename ...TArgs>
	auto variadic_sum(T v1, TArgs... args) -> decltype(v1 + variadic_sum(args...))
	{
		return v1 + variadic_sum(args...);
	}

	template <typename T>
	T variadic_sum(T v)
	{
		return v;
	}
}