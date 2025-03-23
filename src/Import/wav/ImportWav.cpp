#include "Import/wav/ImportWav.hpp"
#include "File/FileFactory.hpp"
#include "File/FilePath.hpp"
#include "Audio/AudioHandle.hpp"
#include <string>

namespace Eternal
{
	namespace Import
	{
		using namespace std;
		using namespace Eternal::Audio;
		using namespace Eternal::FileSystem;

		ImportWav::ImportWav()
		{
		}

		Audio::AudioHandle* ImportWav::Import(_In_ const std::string& InPath, _In_ const FileSystem::FileType& InFileType)
		{
			ETERNAL_ASSERT(InFileType == FileType::FILE_TYPE_SFX_SOUNDS || InFileType == FileType::FILE_TYPE_BGM_SOUNDS);

			if (InPath.size() <= 3 ||
				InPath[InPath.size() - 3] != 'w' ||
				InPath[InPath.size() - 2] != 'a' ||
				InPath[InPath.size() - 1] != 'v')
			{
				return nullptr;
			}

			string FullPath = FilePath::Find(InPath, InFileType);
			if (FullPath.size() <= 0)
				return nullptr;

			FileContent WavFileContent = LoadFileToMemory(FullPath);

			return CreateAudioHandle(InPath, std::move(WavFileContent));
		}
	}
}
