#pragma once

#include "Time/Timer.hpp"

namespace Eternal
{
	namespace Time
	{
		using TimeNanoSecondsT	= uint64_t;
		using TimeMicroSecondsT	= uint64_t;
		using TimeSecondsT		= double;
		
		class Timer
		{
		public:
			static constexpr int TIMER_STACK_SIZE = 1024;

			Timer();
			virtual ~Timer() {}
			static Timer* Get();
			static void ToHumanReadable(_In_ const TimeMicroSecondsT& Value, _Out_ char HumanReadable[24]);

			virtual void Begin() = 0;
			virtual TimeMicroSecondsT End() = 0;
			virtual TimeMicroSecondsT GetTimeMicroSeconds() const = 0;
			TimeMicroSecondsT GetDeltaTimeMicroSeconds() const { return _DeltaTimeMicroSeconds; }
			TimeSecondsT GetDeltaTimeSeconds() const { return _DeltaTimeSeconds; }
			virtual void Update() = 0;

		protected:
			TimeMicroSecondsT	_PreviousTimeMicroSeconds = 0ull;
			TimeMicroSecondsT	_DeltaTimeMicroSeconds = 0ull;
			TimeSecondsT		_DeltaTimeSeconds = 0.0;

		private:
			static Timer*		_Instance;
		};
	}
}
