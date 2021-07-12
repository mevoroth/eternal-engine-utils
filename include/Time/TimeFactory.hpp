#pragma once

namespace Eternal
{
	namespace Time
	{
		class Timer;

		enum class TimeType
		{
			TIME_TYPE_WIN
		};

		Timer* CreateTimer(_In_ const TimeType& InTimeType);
		void DestroyTimer(_Inout_ Timer*& InOutTimer);
	}
}
