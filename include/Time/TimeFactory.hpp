#ifndef _TIME_FACTORY_HPP_
#define _TIME_FACTORY_HPP_

namespace Eternal
{
	namespace Time
	{
		class Time;

		enum TimeType
		{
			WIN
		};

		Time* CreateTime(_In_ const TimeType& TimeTypObj);
	}
}

#endif
