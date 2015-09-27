#ifndef _ATOMIC_INT_
#define _ATOMIC_INT_

namespace Eternal
{
	namespace Parallel
	{
		class AtomicInt
		{
		public:
			inline virtual int Load() = 0;
			inline virtual void Store(_In_ int Value) = 0;
			inline virtual void Add(_In_ int Value = 1) = 0;
			inline virtual void Sub(_In_ int Value = 1) = 0;
		};
	}
}

#endif
