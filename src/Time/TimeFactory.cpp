#include "Time/TimeFactory.hpp"

#include "Time/Windows/WindowsTimer.hpp"
#if ETERNAL_USE_PRIVATE
#include "Time/TimeFactoryPrivate.hpp"
#endif

namespace Eternal
{
	namespace Time
	{
		Timer* CreateTimer(_In_ const TimeType& InTimeType)
		{
#if ETERNAL_PLATFORM_WINDOWS
			if (InTimeType == TimeType::TIME_TYPE_WINDOWS)
				return new WindowsTimer();
#endif

#if ETERNAL_USE_PRIVATE
			return CreateTimerPrivate(InTimeType);
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
