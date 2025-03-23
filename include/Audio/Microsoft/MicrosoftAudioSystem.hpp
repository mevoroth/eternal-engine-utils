#include "Audio/AudioSystem.hpp"

#include "Container/RingBuffer.hpp"
#include <xaudio2.h>
#include <array>
#include <vector>
#include <unordered_map>

struct IMMDeviceEnumerator;
struct IMMDevice;
struct IAudioClient;

namespace Eternal
{
	namespace Audio
	{
		using namespace std;

		class MicrosoftAudioSystemVoiceCallback : public IXAudio2VoiceCallback
		{
		public:

			MicrosoftAudioSystemVoiceCallback();
			~MicrosoftAudioSystemVoiceCallback();

			virtual void OnVoiceProcessingPassStart(_In_ UINT32 InBytesRequired) override;
			virtual void OnVoiceProcessingPassEnd() override;
			virtual void OnStreamEnd() override;
			virtual void OnBufferStart(_In_ void* InBufferContext) override;
			virtual void OnBufferEnd(_In_ void* InBufferContext) override;
			virtual void OnLoopEnd(_In_ void* InBufferContext) override;
			virtual void OnVoiceError(_In_ void* InBufferContext, _In_ HRESULT InError) override;

			HANDLE BufferEndEvent = nullptr;
		};

		class MicrosoftAudioSystem : public AudioSystem
		{
			static constexpr uint32_t SourceVoicesPoolMaxCount	= 1u;
			static constexpr uint32_t AudioHandleQueueSize		= 8u;
			static constexpr uint32_t InvalidVoiceIndex			= ~0u;

			struct AudioHandleQueueReferences
			{
				vector<uint32_t> Queues;
			};

			using SourceVoicesPool			= array<IXAudio2SourceVoice*, SourceVoicesPoolMaxCount>;
			using SourceVoiceCallbacksPool	= array<MicrosoftAudioSystemVoiceCallback, SourceVoicesPoolMaxCount>;
			using AudioHandleQueue			= array<Container::RingBuffer<AudioHandle*, AudioHandleQueueSize>, SourceVoicesPoolMaxCount>;
			using AudioHandleMap			= unordered_map<AudioHandle*, AudioHandleQueueReferences>;

		public:

			MicrosoftAudioSystem();
			~MicrosoftAudioSystem() override;

			virtual void UpdateAudioSystem() override;

			virtual void Play(_In_ AudioHandle* InHandle) override;
			virtual void Stop(_In_ AudioHandle* InHandle) override;

		private:

			IMMDeviceEnumerator*		_MMDeviceEnumerator		= nullptr;
			IMMDevice*					_MMDevice				= nullptr;
			IAudioClient*				_AudioClient			= nullptr;
			IXAudio2*					_XAudio2				= nullptr;
			IXAudio2MasteringVoice*		_XAudio2MasteringVoice	= nullptr;
			SourceVoicesPool			_SourceVoicesPool;
			SourceVoiceCallbacksPool	_SourceVoiceCallbacksPool;
			AudioHandleQueue			_AudioHandleQueues;
			AudioHandleMap				_AudioHandleMap;

		};
	}
}
