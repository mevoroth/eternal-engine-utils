#pragma once

#include <algorithm>
#include <cmath>

namespace Eternal
{
	namespace Math
	{
		static constexpr float PI				= 3.14159265358979323846f;
		static constexpr float PI_2				= PI * 2.0f;
		static constexpr float PI_2_RCP			= 1.0f / PI_2;
		static constexpr float PI_DIV_2			= PI / 2.0f;
		static constexpr float PI_DIV_4			= PI / 4.0f;
		static constexpr float DegreesToRadians	= PI / 180.0f;
		static constexpr float M_TO_KM			= 1.0f / 1000.0f;

		template<typename T>
		constexpr T DivideRoundUp(_In_ T InDividend, _In_ T InDivisor)
		{
			return (InDividend + InDivisor - 1) / InDivisor;
		}

		template<typename T>
		constexpr T Align(_In_ T InDividend, _In_ T InAlignmentSize)
		{
			return DivideRoundUp<T>(InDividend, InAlignmentSize) * InAlignmentSize;
		}

		template<typename T>
		bool IsPowerOfTwo(_In_ T InNumber)
		{
			return InNumber && ((InNumber & (InNumber - 1)) == 0);
		}

		template<typename T>
		constexpr T Sign(_In_ const T& InX)
		{
			return T(T(0) < InX) - (InX < T(0));
		}

		template<typename T>
		T Abs(_In_ const T& InX)
		{
			return std::abs(InX);
		}

		template<typename T>
		constexpr T Min(_In_ const T& In, _In_ const T& InB)
		{
			return std::min<T>(In, InB);
		}

		template<typename T>
		constexpr T Max(_In_ const T& InA, _In_ const T& InB)
		{
			return std::max(InA, InB);
		}

		template<typename T>
		constexpr T Clamp(_In_ const T& InX, _In_ const T& InA, _In_ const T& InB)
		{
			return Max(Min(InX, InB), InA);
		}

		template<typename T>
		constexpr T SafeClamp(_In_ const T& InX, _In_ const T& InA, _In_ const T& InB)
		{
			if (InA < InB)
				std::swap(InA, InB);

			return Clamp(InX, InA, InB);
		}

		template<typename T>
		constexpr T Saturate(_In_ const T& InX)
		{
			return Clamp(InX, T(0.0f), T(1.0f));
		}

		template<typename T>
		constexpr T Lerp(_In_ const T& InA, _In_ const T& InB, _In_ float InX)
		{
			return  InA + (InB - InA) * InX;
		}

		template<typename T>
		constexpr T InverseLerp(_In_ const T& InA, _In_ const T& InB, _In_ const T& InY)
		{
			return (InY - InA) / (InB - InA);
		}

		template<typename T>
		constexpr T Log2(_In_ const T& InX)
		{
			return static_cast<T>(log2(InX));
		}

		template<typename T>
		constexpr T Atan2(_In_ const T& InY, _In_ const T& InX)
		{
			return atan2(InY, InX);
		}

		template<typename T>
		constexpr T Cos(_In_ const T& InAngleInRadians)
		{
			return cos(InAngleInRadians);
		}

		template<typename T>
		constexpr T Sin(_In_ const T& InAngleInRadians)
		{
			return sin(InAngleInRadians);
		}

		template<typename T>
		constexpr T FMod(_In_ const T& InNumerator, _In_ const T& InDenumerator)
		{
			return fmod(InNumerator, InDenumerator);
		}

		template<typename T>
		constexpr T Floor(_In_ const T& InNumber)
		{
			return floor(InNumber);
		}

		int Log2Fast(_In_ uint32_t InX);
	}
}
