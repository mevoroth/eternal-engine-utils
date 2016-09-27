#ifndef _ATOMIC_S32_FACTORY_HPP_
#define _ATOMIC_S32_FACTORY_HPP_

namespace Eternal
{
	namespace Parallel
	{
		class AtomicS32;
		AtomicS32* CreateAtomicS32(_In_ int Value = 0);
	}
}

#endif
