#pragma once

#include <algorithm>

namespace Eternal
{
	namespace Math
	{
		static constexpr float PI = 3.14159265358979323846f;
		static constexpr float DegreesToRadians = PI / 180.0f;

		template<typename T>
		constexpr T DivideRoundUp(_In_ T Dividend, _In_ T Divisor)
		{
			return (Dividend + Divisor - 1) / Divisor;
		}

		template<typename T>
		constexpr T Align(_In_ T Dividend, _In_ T AlignmentSize)
		{
			return DivideRoundUp<T>(Dividend, AlignmentSize) * AlignmentSize;
		}

		template<typename T>
		bool IsPowerOfTwo(_In_ T Number)
		{
			return Number && ((Number & (Number - 1)) == 0);
		}

		template<typename T>
		T Sign(_In_ const T& X)
		{
			return T(T(0) < X) - (X < T(0));
		}

		template<typename T>
		T Abs(_In_ const T& X)
		{
			return std::abs(X);
		}

		template<typename T>
		T Min(_In_ const T& A, _In_ const T& B)
		{
			return std::min<T>(A, B);
		}

		template<typename T>
		T Max(_In_ const T& A, _In_ const T& B)
		{
			return std::max(A, B);
		}

		template<typename T>
		T Clamp(_In_ const T& X, _In_ const T& A, _In_ const T& B)
		{
			return Max(Min(X, B), A);
		}

		template<typename T>
		T SafeClamp(_In_ const T& X, _In_ const T& A, _In_ const T& B)
		{
			if (A < B)
				std::swap(A, B);

			return Clamp(X, A, B);
		}

		template<typename T>
		T Saturate(_In_ const T& X)
		{
			return Clamp(X, T(0.0f), T(1.0f));
		}

		template<typename T>
		T Lerp(_In_ const T& A, _In_ const T& B, _In_ float X)
		{
			return  A + (B - A) * X;
		}

		template<typename T>
		T InverseLerp(_In_ const T& A, _In_ const T& B, _In_ const T& Y)
		{
			return (Y - A) / (B - A);
		}
	}
}
