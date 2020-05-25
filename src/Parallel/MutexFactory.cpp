#include "Parallel/MutexFactory.hpp"

#include "Macros/Macros.hpp"
#include "Parallel/StdMutex.hpp"

#define ETERNAL_USE_STD_MUTEX (1)

namespace Eternal
{
	namespace Parallel
	{
		Mutex* CreateMutex()
		{
#if ETERNAL_USE_STD_MUTEX
			return new StdMutex();
#endif
			ETERNAL_ASSERT(false);
			return nullptr;
		}

		void DestroyMutex(_In_ Mutex*& InMutex)
		{
			delete InMutex;
			InMutex = nullptr;
		}
	}
}
