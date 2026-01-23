#if ETERNAL_PLATFORM_ANDROID

#include "File/Android/AndroidCFile.hpp"

namespace Eternal
{
	namespace FileSystem
	{
		errno_t fopen_s(_Out_ FILE** OutStreamPointer, _In_ const char* InFilePath, _In_ const char* InMode)
		{
			errno_t Result = 0;

			ETERNAL_ASSERT(OutStreamPointer);
			*OutStreamPointer = fopen(InFilePath, InMode);
			if (!*OutStreamPointer)
				Result = errno;
			return Result;
		}
	}
}

#endif
