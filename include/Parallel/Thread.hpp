#ifndef _THREAD_HPP_
#define _THREAD_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Parallel
	{
		class Thread
		{
		public:
			typedef uint32_t (*ThreadFunction)(void*);

			virtual void Create(ThreadFunction Function, void* FunctionParams) = 0;
		};
	}
}

#endif
