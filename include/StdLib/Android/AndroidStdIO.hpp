#pragma once

#if ETERNAL_PLATFORM_ANDROID

#include <cstdio>

template<size_t BufferSize>
inline int sprintf_s(char(&InBuffer)[BufferSize], const char* InFormat, ...)
{
	va_list Parameters;
	va_start(Parameters, InFormat);
	int Result = vsnprintf(InBuffer, BufferSize, InFormat, Parameters);
	va_end(Parameters);
	return Result;
}

inline int sprintf_s(_Out_ char* InBuffer, _In_ size_t InBufferSize, _In_ const char* InFormat, ...)
{
	va_list Parameters;
	va_start(Parameters, InFormat);
	int Result = vsnprintf(InBuffer, InBufferSize, InFormat, Parameters);
	va_end(Parameters);
	return Result;
}

#endif
