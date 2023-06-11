#pragma once

namespace Eternal
{
	namespace Parallel
	{
		class AtomicS32
		{
		public:
			virtual ~AtomicS32() {}
			inline virtual int Load() = 0;
			inline virtual void Store(_In_ int InValue) = 0;
			inline virtual void Add(_In_ int InValue = 1) = 0;
			inline virtual void Sub(_In_ int InValue = 1) = 0;
			inline virtual bool CompareAndSwap(_In_ int InOldValue, _In_ int InNewValue) = 0;
		};
	}
}
