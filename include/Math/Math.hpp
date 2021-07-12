#pragma once

#include <cstdlib>

namespace Eternal
{
	namespace Math
	{
		template<typename T>
		constexpr T DivideRoundUp(T Dividend, T Divisor)
		{
			return (Dividend + Divisor - 1) / Divisor;
		}

		template<typename T>
		constexpr T Align(T Dividend, T AlignmentSize)
		{
			return DivideRoundUp<T>(Dividend, AlignmentSize) * AlignmentSize;
		}

		template<typename T>
		bool IsPowerOfTwo(T Number)
		{
			return Number && ((Number & (Number - 1)) == 0);
		}

		template<typename T>
		T Min(T A, T B)
		{
			return std::min(A, B);
		}

		template<typename T>
		T Max(T A, T B)
		{
			return std::max(A, B);
		}
	}
}
