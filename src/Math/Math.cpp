#include "Math/Math.hpp"

namespace Eternal
{
	namespace Math
	{
		int Log2Fast(_In_ uint32_t InX)
		{
			static constexpr int LogTab[] =
			{
				0,	9,	1,	10,	13,	21,	2,	29,
				11,	14,	16,	18,	22,	25,	3,	30,
				 8,	12,	20,	28,	15,	17,	24,	7,
				19,	27,	23,	6,	26,	5,	4,	31
			};
			ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(LogTab) == sizeof(int) * 8, "LogTab doesn't match");

			InX |= InX >> 1;
			InX |= InX >> 2;
			InX |= InX >> 4;
			InX |= InX >> 8;
			InX |= InX >> 16;
			return LogTab[(uint32_t)(InX * 0x07C4ACDD) >> 27];
		}
	}
}
