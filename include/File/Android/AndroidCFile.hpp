#pragma once

#if ETERNAL_PLATFORM_ANDROID

struct __sFILE;
typedef struct __sFILE FILE;

namespace Eternal
{
	namespace FileSystem
	{
		using errno_t = int;

		errno_t fopen_s(_Out_ FILE** OutStreamPointer, _In_ const char* InFilePath, _In_ const char* InMode);
	}
}

#endif
