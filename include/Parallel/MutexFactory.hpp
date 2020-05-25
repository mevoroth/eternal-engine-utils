#pragma once

namespace Eternal
{
	namespace Parallel
	{
		class Mutex;

		Mutex* CreateMutex();
		void DestroyMutex(_In_ Mutex*& InMutex);
	}
}
