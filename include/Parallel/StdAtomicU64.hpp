#pragma once

#include <atomic>

#include "Parallel/AtomicU64.hpp"

namespace Eternal
{
	namespace Parallel
	{
		using namespace std;

		class StdAtomicU64 : public AtomicU64
		{
		public:
			StdAtomicU64(uint64_t InValue = 0ull)
				: _Value(InValue)
			{
			}
			inline virtual uint64_t Load() override
			{
				return _Value.load();
			}
			inline virtual void Store(uint64_t InValue) override
			{
				_Value.store(InValue);
			}
			inline virtual void Add(_In_ uint64_t InValue = 1) override
			{
				_Value.fetch_add(InValue);
			}
			inline virtual void Sub(_In_ uint64_t InValue = 1) override
			{
				_Value.fetch_sub(InValue);
			}
			inline virtual bool CompareAndSwap(_In_ uint64_t InOldValue, _In_ uint64_t InNewValue) override
			{
				return atomic_compare_exchange_strong(&_Value, &InOldValue, InNewValue);
			}

		private:
			atomic<uint64_t> _Value;
		};
	}
}
