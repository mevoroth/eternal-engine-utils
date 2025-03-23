#pragma once

#include <string>

namespace Eternal
{
	namespace Audio
	{
		class AudioHandle;
	}

	namespace FileSystem
	{
		enum class FileType;
	}

	namespace Import
	{
		class ImportWav
		{
		public:

			ImportWav();

			Audio::AudioHandle* Import(_In_ const std::string& InPath, _In_ const FileSystem::FileType& InFileType);

		};
	}
}
