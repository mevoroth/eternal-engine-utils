#include "Bit/Bit.hpp"

#if ETERNAL_PLATFORM_WINDOWS || ETERNAL_PLATFORM_SCARLETT

#include <intrin.h>

namespace Eternal
{
	namespace Bit
	{
		uint64_t LeadingZeroCount(_In_ uint64_t Value)
		{
			return __lzcnt64(Value);
		}

		uint32_t LeadingZeroCount(_In_ uint32_t Value)
		{
			return __lzcnt(Value);
		}

		uint16_t LeadingZeroCount(_In_ uint16_t Value)
		{
			return __lzcnt16(Value);
		}
	}
}

#endif
