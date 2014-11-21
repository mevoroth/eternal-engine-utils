#include "Time/Win/WinTime.hpp"

#include <cassert>
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace Eternal::Time;

WinTime::WinTime()
	: Time()
{
	if (!QueryPerformanceCounter(&_freq))
	{
		memset(&_freq, 0x0, sizeof(LARGE_INTEGER));
	}
	assert(_freq.QuadPart != 0);
}

void WinTime::Begin()
{
	assert(!_stack.Full());

	LARGE_INTEGER val;
	if (QueryPerformanceCounter(&val))
	{
		_stack.Push(val);
	}
}

TimeT WinTime::End()
{
	assert(!_stack.Empty());

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
