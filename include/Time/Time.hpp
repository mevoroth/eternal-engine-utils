#ifndef _TIME_HPP_
#define _TIME_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Time
	{
		typedef uint64_t	TimeNanoSecondsT;
		typedef uint64_t	TimeMicroSecondsT;
		typedef double		TimeSecondsT;
		class Time
		{
		private:
			static Time* _Inst;
		public:
			Time();
			static Time* Get();
			static void ToHumanReadable(_In_ const TimeMicroSecondsT& Value, _Out_ char HumanReadable[24]);

			virtual void Begin() = 0;
			virtual TimeMicroSecondsT End() = 0;
			virtual TimeMicroSecondsT GetTimeMicroSeconds() const = 0;
			virtual TimeMicroSecondsT GetDeltaTimeMicroSeconds() const = 0;
			virtual TimeSecondsT GetDeltaTimeSeconds() const = 0;
			virtual void Update() = 0;
		};
	}
}

#endif
