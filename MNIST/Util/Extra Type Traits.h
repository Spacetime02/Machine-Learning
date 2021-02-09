#pragma once

#include <type_traits>

namespace extra_traits
{
	template<typename T>
	using remove_const_reference_t = std::remove_const_t<std::remove_reference_t<T>>;

	template<template<typename...> typename Temp, typename Type>
	struct is_specialization : std::false_type {};

	template<template<typename...> class Temp, typename... Ts>
	struct is_specialization<Temp, Temp<Ts...>> : std::true_type {};

	template<typename Type, template<typename...> typename Temp>
	inline constexpr bool is_specialization_v = is_specialization<Type, Temp...>::value;

	template<typename Type, template<typename> typename Temp>
	struct remove_template
	{
		using type = Type;
	};

	template<typename Type, template<typename> typename Temp>
	struct remove_template<Temp<Type>, Temp>
	{
		using type = Type;
	};

	template<typename Type, template<typename> typename Temp>
	using remove_template_t = typename remove_template<Type, Temp>::type;
}
