#include "Time/Win/WinTime.hpp"

#include "Macros/Macros.hpp"
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>

using namespace Eternal::Time;

WinTime::WinTime()
	: Time()
{
	if (!QueryPerformanceCounter(&_freq))
	{
		memset(&_freq, 0x0, sizeof(LARGE_INTEGER));
	}
	ETERNAL_ASSERT(_freq.QuadPart != 0);
}

void WinTime::Begin()
{
	ETERNAL_ASSERT(!_stack.Full());

	LARGE_INTEGER val;
	if (QueryPerformanceCounter(&val))
	{
		_stack.Push(val);
	}
}

TimeT WinTime::End()
{
	ETERNAL_ASSERT(!_stack.Empty());

	LARGE_INTEGER val;
	if (QueryPerformanceCounter(&val))
	{
		val.QuadPart = val.QuadPart - _stack.Head().QuadPart;
		_stack.Pop();
		return val.QuadPart * 1000l / _freq.QuadPart;
	}

	return -1l;
}

TimeT WinTime::GetTime() const
{
	LARGE_INTEGER val;
	if (QueryPerformanceCounter(&val))
	{
		return val.QuadPart * 1000l / _freq.QuadPart;
	}
	return -1l;
}
