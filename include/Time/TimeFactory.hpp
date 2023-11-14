#pragma once

namespace Eternal
{
	namespace Time
	{
		class Timer;

		enum class TimeType
		{
			TIME_TYPE_WINDOWS
#if ETERNAL_USE_PRIVATE
			#include "Time/TimeFactoryTimeTypePrivate.hpp"
#else
			, TIME_TYPE_DEFAULT = TIME_TYPE_WINDOWS
#endif
		};

		Timer* CreateTimer(_In_ const TimeType& InTimeType);
		void DestroyTimer(_Inout_ Timer*& InOutTimer);
	}
}
