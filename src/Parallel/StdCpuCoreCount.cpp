#include "Parallel/CpuCoreCount.hpp"

#include <thread>

using namespace std;

namespace Eternal
{
	namespace Parallel
	{
		uint32_t CpuCoreCount()
		{
			return thread::hardware_concurrency();
		}
	}
}
