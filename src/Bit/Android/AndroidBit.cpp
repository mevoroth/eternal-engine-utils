#include "Bit/Bit.hpp"

#if ETERNAL_PLATFORM_ANDROID

namespace Eternal
{
	namespace Bit
	{
		uint64_t LeadingZeroCount(_In_ uint64_t Value)
		{
			return __builtin_clzll(Value);
		}

		uint32_t LeadingZeroCount(_In_ uint32_t Value)
		{
			return __builtin_clz(Value);
		}

		uint16_t LeadingZeroCount(_In_ uint16_t Value)
		{
			return static_cast<uint16_t>(__builtin_clz(Value));
		}
	}
}

#endif
