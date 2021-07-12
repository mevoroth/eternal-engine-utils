#include "Parallel/AtomicS32Factory.hpp"

#include "Parallel/StdAtomicS32.hpp"

namespace Eternal
{
	namespace Parallel
	{
		AtomicS32* CreateAtomicS32(_In_ int Value /*= 0*/)
		{
			return new StdAtomicS32(Value);
		}

		void DestroyAtomicS32(_Inout_ AtomicS32*& InOutAtomic)
		{
			delete InOutAtomic;
			InOutAtomic = nullptr;
		}
	}
}
