#include "Time/TimeFactory.hpp"

#include "Macros/Macros.hpp"
#include "Time/Win/WinTime.hpp"

namespace Eternal
{
	namespace Time
	{
		Time* CreateTime(_In_ const TimeType& TimeTypObj)
		{
			if (TimeTypObj == WIN)
				return new WinTime();
			ETERNAL_ASSERT(false);
			return nullptr;
		}
	}
}
