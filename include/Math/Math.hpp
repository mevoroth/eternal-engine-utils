#pragma once

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
		bool IsPowerOfTwo(T Number)
		{
			return Number && ((Number & (Number - 1)) == 0);
		}
	}
}
