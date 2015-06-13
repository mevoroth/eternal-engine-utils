#ifndef _STD_MUTEX_HPP_
#define _STD_MUTEX_HPP_

#include "Mutex.hpp"

#include <mutex>

using namespace std;

namespace Eternal
{
	namespace Parallel
	{
		class StdMutex : public Mutex
		{
			friend class StdConditionVariable;
		public:
			virtual bool TryLock() override;
			virtual void Lock() override;
			virtual void Unlock() override;

		private:
			mutex _Mutex;
		};
	}
}

#endif
