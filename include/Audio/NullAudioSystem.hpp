#pragma once

#include "Audio/AudioSystem.hpp"

namespace Eternal
{
	namespace Audio
	{
		class NullAudioSystem final : public AudioSystem
		{
		public:

			virtual void UpdateAudioSystem() override final;

			virtual void Play(_In_ AudioHandle* InHandle) override final;
			virtual void Stop(_In_ AudioHandle* InHandle) override final;

		};
	}
}
