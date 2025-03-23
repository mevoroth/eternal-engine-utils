#include "Audio/AudioSystemFactory.hpp"
#include "Audio/Microsoft/MicrosoftAudioSystem.hpp"

namespace Eternal
{
	namespace Audio
	{
		AudioSystem* CreateAudioSystem()
		{
#if ETERNAL_PLATFORM_WINDOWS || ETERNAL_PLATFORM_SCARLETT
			return new MicrosoftAudioSystem();
#endif

			ETERNAL_BREAK();
			return nullptr;
		}
	}
}
