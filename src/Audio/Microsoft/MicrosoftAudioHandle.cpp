#include "Audio/Microsoft/MicrosoftAudioHandle.hpp"

#include "Audio/AudioHandle.hpp"
#include "include/Wav.h"
#include "Math/Math.hpp"
#include "Log/Log.hpp"

namespace Eternal
{
	namespace Audio
	{
		MicrosoftAudioHandle::MicrosoftAudioHandle(_In_ const std::string& InAudioKey, _In_ FileContent&& InFileContent)
			: AudioFileContent(std::move(InFileContent))
			, AudioMemoryFile(AudioFileContent)
		{
			AudioMemoryFile.Open(FileOpenMode::FILE_OPEN_MODE_READ);

			Wav::Header WavHeader;
			AudioMemoryFile.Serialize(WavHeader);

			Wav::Data WavLISTData;
			AudioMemoryFile.Serialize(WavLISTData);

			ETERNAL_ASSERT(WavLISTData.DATAMarker == Wav::MarkerLIST);

			if (DebugMicrosoftAudioHandle)
			{
				LogWrite(LogInfo, LogAudio, string("=== Loaded ") + InAudioKey);

				uint64_t ChunkInitialOffset = AudioMemoryFile.Tell();

				Wav::Marker WavINFOMarker;
				AudioMemoryFile.Serialize(WavINFOMarker);

				ETERNAL_ASSERT(WavINFOMarker == Wav::MarkerINFO);

				char INFOPayload[1024] = {};
				uint64_t AlignedDataSize = 0;

				using namespace Eternal::Math;

				uint64_t CurrentOffset = 0ull;

				while (CurrentOffset < WavLISTData.DataSize)
				{
					Wav::Data WavSubINFOData;
					AudioMemoryFile.Serialize(WavSubINFOData);
					AlignedDataSize = Align<uint64_t>(WavSubINFOData.DataSize, 2ull);
					AudioMemoryFile.Read(reinterpret_cast<uint8_t*>(INFOPayload), AlignedDataSize);
					CurrentOffset = AudioMemoryFile.Tell() - ChunkInitialOffset;

					LogWrite(LogInfo, LogAudio, string("\t\t") + string(WavSubINFOData.DATAMarker.Bytes, 4) + string(": ") + string(INFOPayload));
				}
				
				ETERNAL_ASSERT(CurrentOffset == WavLISTData.DataSize);
			}
			else
			{
				AudioMemoryFile.Seek(WavLISTData.DataSize, FileCursor::FILE_CURSOR_CUR);
			}

			Wav::Data WavData;
			AudioMemoryFile.Serialize(WavData);

			XAudio2Buffer.pAudioData	= AudioFileContent.Content + AudioMemoryFile.Tell();
			XAudio2Buffer.AudioBytes	= WavData.DataSize;
			XAudio2Buffer.Flags			= XAUDIO2_END_OF_STREAM;

			//ETERNAL_BREAK();
		}

		MicrosoftAudioHandle::~MicrosoftAudioHandle()
		{
			AudioMemoryFile.Close();
		}

		//////////////////////////////////////////////////////////////////////////

		AudioHandle* CreateAudioHandle(_In_ const std::string& InAudioKey, _In_ FileContent&& InFileContent)
		{
			return new MicrosoftAudioHandle(InAudioKey, std::move(InFileContent));
		}
	}
}
