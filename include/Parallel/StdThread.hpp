#ifndef _STD_THREAD_HPP_
#define _STD_THREAD_HPP_

#include "Parallel/Thread.hpp"

#include <thread>

using namespace std;

namespace Eternal
{
	namespace Parallel
	{
		class StdThread : public Thread
		{
		public:
			virtual void Create(ThreadFunction Function, void* FunctionParams) override;

		private:
			thread _Thread;
		};
	}
}

#endif
