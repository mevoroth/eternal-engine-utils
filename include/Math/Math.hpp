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
	}
}
