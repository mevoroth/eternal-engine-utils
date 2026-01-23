#pragma once

#include "Time/TimeHeader.hpp"
#include "Container/Stack.hpp"

namespace Eternal
{
	namespace Time
	{
		using namespace Eternal::Container;

		using TimeNanoSecondsT	= uint64_t;
		using TimeMicroSecondsT	= uint64_t;
		using TimeSecondsT		= double;
		
		class Timer
		{
		public:
			static constexpr int TIMER_STACK_SIZE				= 128;
			static constexpr double SecondsToMicroSeconds		= 1000000.0;
			static constexpr double MicroSecondsToSeconds		= 1.0 / SecondsToMicroSeconds;
			static constexpr double NanoSecondsToMicroSeconds	= 0.001;

			Timer();
			virtual ~Timer() {}
			static Timer* Get();
			static void ToHumanReadable(_In_ const TimeMicroSecondsT& Value, _Out_ char HumanReadable[24]);

			virtual void Begin() = 0;
			virtual TimeMicroSecondsT End() = 0;
			virtual TimeMicroSecondsT GetTimeMicroSeconds() const = 0;
			TimeSecondsT GetTimeSeconds() const;

			TimeMicroSecondsT GetDeltaTimeMicroSeconds() const { return _DeltaTimeMicroSeconds; }
			TimeSecondsT GetDeltaTimeSeconds() const { return _DeltaTimeSeconds; }
			void Update();

		protected:
			Stack<TimeInternalType, TIMER_STACK_SIZE>	_TimerStack;
			TimeMicroSecondsT							_PreviousTimeMicroSeconds	= 0ull;
			TimeMicroSecondsT							_DeltaTimeMicroSeconds		= 0ull;
			TimeSecondsT								_DeltaTimeSeconds			= 0.0;
			FrequencyInternalType						_Frequency					= {};

		private:
			static Timer*								_Instance;
		};
	}
}
