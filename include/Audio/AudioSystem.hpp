#pragma once

namespace Eternal
{
	namespace Audio
	{
		struct AudioHandle;

		class AudioSystem
		{
		public:

			AudioSystem();
			virtual ~AudioSystem() {}

			virtual void UpdateAudioSystem() = 0;

			virtual void Play(_In_ AudioHandle* InHandle) = 0;
			virtual void Stop(_In_ AudioHandle* InHandle) = 0;

		};
	}
}
