#pragma once

namespace Eternal
{
	namespace Time
	{
		class Timer;

		enum class TimeType
		{
#if ETERNAL_PLATFORM_WINDOWS
			TIME_TYPE_WINDOWS
#endif
#if ETERNAL_USE_PRIVATE
			#include "Time/TimeFactoryTimeTypePrivate.hpp"
#else

	#if ETERNAL_PLATFORM_WINDOWS
			, TIME_TYPE_DEFAULT = TIME_TYPE_WINDOWS
	#endif

#endif
		};

		Timer* CreateTimer(_In_ const TimeType& InTimeType);
		void DestroyTimer(_Inout_ Timer*& InOutTimer);
	}
}
