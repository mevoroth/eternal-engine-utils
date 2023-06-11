#pragma once

namespace Eternal
{
	namespace Parallel
	{
		class Mutex
		{
		public:
			virtual ~Mutex() {}
			virtual bool TryLock() = 0;
			virtual void Lock() = 0;
			virtual void Unlock() = 0;
		};
	}
}
