#ifndef _MUTEX_AUTO_LOCK_HPP_
#define _MUTEX_AUTO_LOCK_HPP_

#include "Macros/Macros.hpp"
#include "Mutex.hpp"

namespace Eternal
{
	namespace Parallel
	{
		class MutexAutoLock
		{
		public:
			inline MutexAutoLock(Mutex* MutexObj)
				: _Mutex(MutexObj)
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

#endif
