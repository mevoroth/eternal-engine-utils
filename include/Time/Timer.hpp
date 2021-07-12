#pragma once

namespace Eternal
{
	namespace Time
	{
		using TimeNanoSecondsT	= uint64_t;
		using TimeMicroSecondsT	= uint64_t;
		using TimeSecondsT		= double;
		
		class Timer
		{
		private:
			static Timer* _Inst;
		public:
			Timer();
			virtual ~Timer() {}
			static Timer* Get();
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
