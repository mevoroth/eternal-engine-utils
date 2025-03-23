#pragma once

#include "Audio/AudioHandle.hpp"
#include "File/FileFactory.hpp"
#include "File/MemoryFile.hpp"
#include <xaudio2.h>

namespace Eternal
{
	namespace Audio
	{
		using namespace FileSystem;

		static constexpr bool DebugMicrosoftAudioHandle = true;

		struct MicrosoftAudioHandle : public AudioHandle
		{
			MicrosoftAudioHandle(_In_ const std::string& InAudioKey, _In_ FileContent&& InFileContent);
			~MicrosoftAudioHandle();

			XAUDIO2_BUFFER	XAudio2Buffer = {};
			FileContent		AudioFileContent;
			MemoryFile		AudioMemoryFile;
		};
	}
}
