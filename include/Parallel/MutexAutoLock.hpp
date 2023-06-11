#pragma once

#include "Mutex.hpp"

namespace Eternal
{
	namespace Parallel
	{
		class MutexAutoLock
		{
		public:
			inline MutexAutoLock(Mutex* InMutex)
				: _Mutex(InMutex)
			{
				ETERNAL_ASSERT(_Mutex);
				_Mutex->Lock();
			}
			inline ~MutexAutoLock()
			{
				_Mutex->Unlock();
			}
		private:
			Mutex* _Mutex = nullptr;
		};
	}
}
