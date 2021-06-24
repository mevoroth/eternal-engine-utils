#ifndef _ATOMIC_S32_HPP_
#define _ATOMIC_S32_HPP_

namespace Eternal
{
	namespace Parallel
	{
		class AtomicS32
		{
		public:
			virtual ~AtomicS32() {}
			inline virtual int Load() = 0;
			inline virtual void Store(_In_ int Value) = 0;
			inline virtual void Add(_In_ int Value = 1) = 0;
			inline virtual void Sub(_In_ int Value = 1) = 0;
			inline virtual bool CompareAndSwap(_In_ int OldValue, _In_ int NewValue) = 0;
		};
	}
}

#endif
