#include "Audio/AudioSystemFactory.hpp"
#include "Audio/Microsoft/MicrosoftAudioSystem.hpp"
#include "Audio/NullAudioSystem.hpp"
#include "Log/Log.hpp"

namespace Eternal
{
	namespace Audio
	{
		AudioSystem* CreateAudioSystem()
		{
			using namespace Eternal::LogSystem;

#if ETERNAL_PLATFORM_WINDOWS || ETERNAL_PLATFORM_SCARLETT
			return new MicrosoftAudioSystem();
#endif

			LogWrite(LogWarning, LogAudio, "[Audio::CreateAudioSystem]No AudioSystem implemented for platform " ETERNAL_PLATFORM_NAME);
			return new NullAudioSystem();
		}
	}
}
