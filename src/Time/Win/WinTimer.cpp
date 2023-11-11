#if ETERNAL_PLATFORM_WINDOWS

#include "Time/Win/WinTimer.hpp"

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>

using namespace Eternal::Time;

WinTimer::WinTimer()
	: Timer()
{
	if (!QueryPerformanceFrequency(&_Freq))
	{
		memset(&_Freq, 0x0, sizeof(LARGE_INTEGER));
	}
	ETERNAL_ASSERT(_Freq.QuadPart != 0);

	_PreviousTimeMicroSeconds = GetTimeMicroSeconds();
}

void WinTimer::Begin()
{
	ETERNAL_ASSERT(!_Stack.Full());

	LARGE_INTEGER Value;
	if (QueryPerformanceCounter(&Value))
	{
		_Stack.Push(Value);
	}
}

TimeMicroSecondsT WinTimer::End()
{
	ETERNAL_ASSERT(!_Stack.Empty());

	LARGE_INTEGER Value;
	if (QueryPerformanceCounter(&Value))
	{
		Value.QuadPart = Value.QuadPart - _Stack.Head().QuadPart;
		_Stack.Pop();
		return Value.QuadPart * 1000000ull / _Freq.QuadPart;
	}

	return ~0ull;
}

TimeMicroSecondsT WinTimer::GetTimeMicroSeconds() const
{
	LARGE_INTEGER Value;
	if (QueryPerformanceCounter(&Value))
	{
		return Value.QuadPart * 1000000ull / _Freq.QuadPart;
	}
	return ~0ull;
}

TimeMicroSecondsT WinTimer::GetDeltaTimeMicroSeconds() const
{
	return _DeltaTimeMicroSeconds;
}

TimeSecondsT WinTimer::GetDeltaTimeSeconds() const
{
	return _DeltaTimeSeconds;
}

void WinTimer::Update()
{
	TimeMicroSecondsT PreviousTimeMicroSeconds = _PreviousTimeMicroSeconds;
	_PreviousTimeMicroSeconds = GetTimeMicroSeconds();
	_DeltaTimeMicroSeconds = _PreviousTimeMicroSeconds - PreviousTimeMicroSeconds;
	_DeltaTimeSeconds = (double)_DeltaTimeMicroSeconds / 1000000.0;
}

#endif
