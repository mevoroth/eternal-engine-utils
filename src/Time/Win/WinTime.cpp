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
	if (!QueryPerformanceFrequency(&_Freq))
	{
		memset(&_Freq, 0x0, sizeof(LARGE_INTEGER));
	}
	ETERNAL_ASSERT(_Freq.QuadPart != 0);
}

void WinTime::Begin()
{
	ETERNAL_ASSERT(!_Stack.Full());

	LARGE_INTEGER Value;
	if (QueryPerformanceCounter(&Value))
	{
		_Stack.Push(Value);
	}
}

TimeT WinTime::End()
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

TimeT WinTime::GetTime() const
{
	LARGE_INTEGER val;
	if (QueryPerformanceCounter(&val))
	{
		return val.QuadPart * 1000000ull / _Freq.QuadPart;
	}
	return ~0ull;
}

TimeT WinTime::GetDeltaTime() const
{
	return _DeltaTime;
}

void WinTime::Update()
{
	TimeT PreviousTime = _PreviousTime;
	_PreviousTime = GetTime();
	_DeltaTime = PreviousTime - _PreviousTime;
}
