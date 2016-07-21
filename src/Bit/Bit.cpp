#include "Bit/Bit.hpp"

#include <intrin.h>

namespace Eternal
{
	namespace Bit
	{
		uint64_t LeadingZeroCount(_In_ uint64_t Value)
		{
			return __lzcnt64(Value);
		}
	}
}
