#ifndef _THREAD_HPP_
#define _THREAD_HPP_

#include <cstdint>
#include "Time/Time.hpp"

namespace Eternal
{
	namespace Parallel
	{
		using namespace Eternal::Time;

		class Thread
		{
		public:
			typedef uint32_t(*ThreadFunction)(void*);

			virtual void Create(ThreadFunction Function, void* FunctionParams) = 0;
			virtual void Sleep(TimeT Milliseconds) = 0;
		};
	}
}

#endif
