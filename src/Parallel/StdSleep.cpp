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
			this_thread::sleep_for(chrono::nanoseconds(NanoSeconds));
		}
	}
}
