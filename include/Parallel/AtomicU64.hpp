#ifndef _ATOMIC_U64_HPP_
#define _ATOMIC_U64_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Parallel
	{
		class AtomicU64
		{
		public:
			inline virtual uint64_t Load() = 0;
			inline virtual void Store(_In_ uint64_t Value) = 0;
			inline virtual void Add(_In_ uint64_t Value = 1) = 0;
			inline virtual void Sub(_In_ uint64_t Value = 1) = 0;
			inline virtual bool CompareAndSwap(_In_ uint64_t OldValue, _In_ uint64_t NewValue) = 0;
		};
	}
}

#endif
