#pragma once

namespace Eternal
{
	namespace Parallel
	{
		class AtomicS32;

		AtomicS32* CreateAtomicS32(_In_ int Value = 0);
		void DestroyAtomicS32(_Inout_ AtomicS32*& InOutAtomic);
	}
}
