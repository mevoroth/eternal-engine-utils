#ifndef _STD_ATOMIC_S32_HPP_
#define _STD_ATOMIC_S32_HPP_

#include <atomic>

#include "AtomicS32.hpp"

using namespace std;

namespace Eternal
{
	namespace Parallel
	{
		class StdAtomicS32 : public AtomicS32
		{
		public:
			StdAtomicS32(int Value = 0)
				: _Value(Value)
			{
			}
			inline virtual int Load() override
			{
				return _Value.load();
			}
			inline virtual void Store(int Value) override
			{
				_Value.store(Value);
			}
			inline virtual void Add(_In_ int Value = 1) override
			{
				_Value.fetch_add(Value);
			}
			inline virtual void Sub(_In_ int Value = 1) override
			{
				_Value.fetch_sub(Value);
			}
			inline virtual bool CompareAndSwap(_In_ int OldValue, _In_ int NewValue) override
			{
				return atomic_compare_exchange_strong(&_Value, &OldValue, NewValue);
			}

		private:
			atomic<int> _Value;
		};
	}
}

#endif
