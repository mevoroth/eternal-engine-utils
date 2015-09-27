#ifndef _STD_ATOMIC_INT_
#define _STD_ATOMIC_INT_

#include <atomic>

#include "AtomicInt.hpp"

using namespace std;

namespace Eternal
{
	namespace Parallel
	{
		class StdAtomicInt : public AtomicInt
		{
		public:
			StdAtomicInt(int Value)
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

		private:
			atomic<int> _Value;
		};
	}
}

#endif
