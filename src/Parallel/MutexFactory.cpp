#include "Parallel/MutexFactory.hpp"

#include "Parallel/StdMutex.hpp"

namespace Eternal
{
	namespace Parallel
	{
		Mutex* CreateMutex()
		{
#if ETERNAL_USE_STD_PARALLEL
			return new StdMutex();
#endif
			ETERNAL_BREAK();
			return nullptr;
		}

		void DestroyMutex(_In_ Mutex*& InMutex)
		{
			delete InMutex;
			InMutex = nullptr;
		}
	}
}
