#pragma once

#include <string>
#include <unordered_map>

namespace Eternal
{
	namespace Audio
	{
		struct AudioHandle;
	}

	namespace Resources
	{
		using namespace std;

		using AudioCache = Audio::AudioHandle*;

		using AudioKey = string;
		using AudioCacheStorage = unordered_map<AudioKey, AudioCache>;

		class AudioFactory
		{
		public:
			static AudioCache EmptyAudio;

			void CreateAudioCacheEntry(_In_ const AudioKey& InKey);
			AudioCache& GetAudioCache(_In_ const AudioKey& InKey);
			bool AudioExists(_In_ const AudioKey& InKey) const;

			const AudioCacheStorage& GetAudio() const { return _Audio; }

		private:
			AudioCacheStorage _Audio;
		};
	}
}
