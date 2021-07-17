#include "Parallel/Sleep.hpp"

#include <thread>
#include <chrono>

using namespace std;

namespace Eternal
{
	namespace Parallel
	{
		void Sleep(const TimeNanoSecondsT& NanoSeconds)
		{
			ETERNAL_PROFILER(INFO)();
			this_thread::sleep_for(chrono::nanoseconds(NanoSeconds));
		}
	}
}
