#pragma once

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
			StdAtomicS32(int InValue = 0)
				: _Value(InValue)
			{
			}
			inline virtual int Load() override
			{
				return _Value.load();
			}
			inline virtual void Store(int InValue) override
			{
				_Value.store(InValue);
			}
			inline virtual int Add(_In_ int InValue = 1) override
			{
				return _Value.fetch_add(InValue);
			}
			inline virtual int Sub(_In_ int InValue = 1) override
			{
				return _Value.fetch_sub(InValue);
			}
			inline virtual bool CompareAndSwap(_In_ int InOldValue, _In_ int InNewValue) override
			{
				return atomic_compare_exchange_strong(&_Value, &InOldValue, InNewValue);
			}

		private:
			atomic<int> _Value;
		};
	}
}
