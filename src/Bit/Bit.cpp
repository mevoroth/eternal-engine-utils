#include "Bit/Bit.hpp"

#include <intrin.h>

namespace Eternal
{
	namespace Bit
	{
		u64 LeadingZeroCount(_In_ u64 Value)
		{
			return __lzcnt64(Value);
		}
	}
}
