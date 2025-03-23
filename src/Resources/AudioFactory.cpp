#include "Resources/AudioFactory.hpp"

namespace Eternal
{
	namespace Resources
	{
		AudioCache AudioFactory::EmptyAudio = nullptr;

		void AudioFactory::CreateAudioCacheEntry(_In_ const AudioKey& InKey)
		{
			AudioCacheStorage::iterator FoundAudio = _Audio.find(InKey);
			if (FoundAudio == _Audio.cend())
				_Audio.insert(std::make_pair(InKey, AudioCache()));
		}

		AudioCache& AudioFactory::GetAudioCache(_In_ const AudioKey& InKey)
		{
			AudioCacheStorage::iterator FoundAudio = _Audio.find(InKey);
			if (FoundAudio != _Audio.cend())
				return FoundAudio->second;

			ETERNAL_BREAK();
			return EmptyAudio;
		}
		
		bool AudioFactory::AudioExists(_In_ const AudioKey& InKey) const
		{
			AudioCacheStorage::const_iterator FoundAudio = _Audio.find(InKey);
			return FoundAudio != _Audio.cend();
		}
	}
}
