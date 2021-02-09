#pragma once

#include <algorithm>

namespace func
{
	template<typename T = void>
	struct left_shift
	{
		inline constexpr T operator()(const T& lhs, const T& rhs) const
		{
			return lhs << rhs;
		}
	};

	template<>
	struct left_shift<void>
	{
		template<typename T, typename U>
		inline constexpr auto operator()(const T& lhs, const U& rhs) const -> decltype(lhs << rhs)
		{
			return lhs << rhs;
		}
	};

	template<typename T = void>
	struct right_shift
	{
		inline constexpr T operator()(const T& lhs, const T& rhs) const
		{
			return lhs >> rhs;
		}
	};

	template<>
	struct right_shift<void>
	{
		template<typename T, typename U>
		inline constexpr auto operator()(const T& lhs, const U& rhs) const -> decltype(lhs >> rhs)
		{
			return lhs >> rhs;
		}
	};

	template<typename T = void, typename Comp = std::less<T>>
	struct min
	{
		Comp comp{};

		inline constexpr min() = default;
		inline constexpr min(const Comp & comp) : comp(comp) {}

		inline constexpr const T& operator()(const T& lhs, const T& rhs) const
		{
			return comp(rhs, lhs) ? rhs : lhs;
		}
	};

	template<>
	struct min<>
	{
		template<typename T, typename U>
		inline constexpr auto operator()(const T& lhs, const U& rhs) const -> decltype(rhs < lhs ? rhs : lhs)
		{
			return rhs < lhs ? rhs : lhs;
		}
	};

	template<typename T = void, typename Comp = std::less<T>>
	struct max
	{
		Comp comp{};

		inline constexpr max() = default;
		inline constexpr max(const Comp& comp) : comp(comp) {}

		inline constexpr const T& operator()(const T& lhs, const T& rhs) const
		{
			return comp(lhs, rhs) ? rhs : lhs;
		}
	};

	template<>
	struct max<>
	{
		template<typename T, typename U>
		inline constexpr auto operator()(const T& lhs, const U& rhs) const -> decltype(rhs < lhs ? rhs : lhs)
		{
			return lhs < rhs ? rhs : lhs;
		}
	};
}