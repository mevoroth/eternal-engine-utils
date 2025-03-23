#pragma once

#include <string>

namespace Eternal
{
	namespace FileSystem
	{
		struct FileContent;
	}

	namespace Audio
	{
		using namespace FileSystem;

		struct AudioHandle
		{
			virtual ~AudioHandle() {}
		};

		AudioHandle* CreateAudioHandle(_In_ const std::string& InAudioKey, _In_ FileContent&& InFileContent);
	}
}
