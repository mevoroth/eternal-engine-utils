#include "Time/TimeFactory.hpp"

#include "Time/Win/WinTimer.hpp"

namespace Eternal
{
	namespace Time
	{
		Timer* CreateTimer(_In_ const TimeType& InTimeType)
		{
		#if ETERNAL_PLATFORM_WINDOWS
			if (InTimeType == TimeType::TIME_TYPE_WIN)
				return new WinTimer();
		#endif
			ETERNAL_BREAK();
			return nullptr;
		}

		void DestroyTimer(_Inout_ Timer*& InOutTimer)
		{
			delete InOutTimer;
			InOutTimer = nullptr;
		}
	}
}
