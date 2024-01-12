#pragma once

namespace Eternal
{
	namespace Parallel
	{
		class AtomicU64
		{
		public:
			inline virtual uint64_t Load() = 0;
			inline virtual void Store(_In_ uint64_t InValue) = 0;
			inline virtual uint64_t Add(_In_ uint64_t InValue = 1) = 0;
			inline virtual uint64_t Sub(_In_ uint64_t InValue = 1) = 0;
			inline virtual bool CompareAndSwap(_In_ uint64_t InOldValue, _In_ uint64_t InNewValue) = 0;
		};
	}
}
