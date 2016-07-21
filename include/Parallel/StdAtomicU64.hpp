#ifndef _STD_ATOMIC_U64_HPP_
#define _STD_ATOMIC_U64_HPP_

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
			StdAtomicU64(uint64_t Value = 0ull)
				: _Value(Value)
			{
			}
			inline virtual uint64_t Load() override
			{
				return _Value.load();
			}
			inline virtual void Store(uint64_t Value) override
			{
				_Value.store(Value);
			}
			inline virtual void Add(_In_ uint64_t Value = 1) override
			{
				_Value.fetch_add(Value);
			}
			inline virtual void Sub(_In_ uint64_t Value = 1) override
			{
				_Value.fetch_sub(Value);
			}
			inline virtual bool CompareAndSwap(_In_ uint64_t OldValue, _In_ uint64_t NewValue) override
			{
				return atomic_compare_exchange_strong(&_Value, &OldValue, NewValue);
			}

		private:
			atomic<uint64_t> _Value;
		};
	}
}

#endif
