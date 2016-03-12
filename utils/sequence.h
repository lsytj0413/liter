#pragma once

#include <tuple>

namespace liter
{
	template <size_t... Seq>
	struct sequence
	{

	};

	template <size_t End, size_t Beg = 0>
	struct make_sequence
	{
	private:
		template <typename T, size_t... Seq>
		struct make_sequence_imp
		{

		};

		template <size_t Beg, size_t End, size_t... Seq>
		struct make_sequence_imp<sequence<Seq...>, Beg, End>
		{
			using type = typename make_sequence_imp<sequence<Seq..., Beg>, Beg + 1, End>::type;
		};

		template <size_t End, size_t... Seq>
		struct make_sequence_imp<sequence<Seq...>, End, End>
		{
			using type = sequence<Seq...>;
		};

	public:
		using type = typename make_sequence_imp<sequence<>, Beg, End>::type;
	};

	namespace detail
	{
		template <typename... TArgs, size_t... Seq>
		void functor_imp(std::tuple<TArgs...> args, sequence<Seq...>)
		{
			std::get<0>(args)(std::get<Seq>(args)...);
		}

		template <typename... TArgs>
		void functor_imp(std::tuple<TArgs...> args)
		{

			functor_imp(args,
				make_sequence<std::tuple_size<decltype(args)>::value, 1>::type());
		}
	}

	template <typename... TArgs>
	void functor_call(TArgs... args)
	{
		detail::functor_imp(std::tuple<TArgs...>(args...));
	}
}