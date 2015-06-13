#ifndef _MUTEX_HPP_
#define _MUTEX_HPP_

namespace Eternal
{
	namespace Parallel
	{
		class Mutex
		{
		public:
			virtual bool TryLock() = 0;
			virtual void Lock() = 0;
			virtual void Unlock() = 0;
		};
	}
}

#endif
