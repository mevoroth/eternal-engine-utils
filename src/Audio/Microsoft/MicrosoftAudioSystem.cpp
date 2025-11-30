#include "Audio/Microsoft/MicrosoftAudioSystem.hpp"

#if ETERNAL_PLATFORM_WINDOWS

#include "Audio/Microsoft/MicrosoftAudioHandle.hpp"
#include "Log/Log.hpp"
#include <Objbase.h>
#include <xaudio2.h>

#define COBJMACROS (1)
#include <mmdeviceapi.h>
#include <Audioclient.h>

namespace Eternal
{
	namespace Audio
	{
		static void VerifyHResult(HRESULT InHResult)
		{
			ETERNAL_ASSERT(InHResult >= S_OK);
		}

		//////////////////////////////////////////////////////////////////////////

		MicrosoftAudioSystemVoiceCallback::MicrosoftAudioSystemVoiceCallback()
			: BufferEndEvent(CreateEvent(nullptr, FALSE, FALSE, nullptr))
		{
		}

		MicrosoftAudioSystemVoiceCallback::~MicrosoftAudioSystemVoiceCallback()
		{
			CloseHandle(BufferEndEvent);
		}

		void MicrosoftAudioSystemVoiceCallback::OnVoiceProcessingPassStart(_In_ UINT32 InBytesRequired)
		{
			//ETERNAL_BREAK();
		}

		void MicrosoftAudioSystemVoiceCallback::OnVoiceProcessingPassEnd()
		{
			//ETERNAL_BREAK();
		}

		void MicrosoftAudioSystemVoiceCallback::OnStreamEnd()
		{
			//ETERNAL_BREAK();
		}

		void MicrosoftAudioSystemVoiceCallback::OnBufferStart(_In_ void* InBufferContext)
		{
			//ETERNAL_BREAK();
		}

		void MicrosoftAudioSystemVoiceCallback::OnBufferEnd(_In_ void* InBufferContext)
		{
			//ETERNAL_BREAK();
			SetEvent(BufferEndEvent);
		}

		void MicrosoftAudioSystemVoiceCallback::OnLoopEnd(_In_ void* InBufferContext)
		{
			//ETERNAL_BREAK();
		}
		
		void MicrosoftAudioSystemVoiceCallback::OnVoiceError(_In_ void* InBufferContext, _In_ HRESULT InError)
		{
			//ETERNAL_BREAK();
		}

		//////////////////////////////////////////////////////////////////////////

		constexpr uint32_t MicrosoftAudioSystem::SourceVoicesPoolMaxCount;
		constexpr uint32_t MicrosoftAudioSystem::AudioHandleQueueSize;
		constexpr uint32_t MicrosoftAudioSystem::InvalidVoiceIndex;

		MicrosoftAudioSystem::MicrosoftAudioSystem()
			: AudioSystem()
		{
			VerifyHResult(CoInitializeEx(nullptr, COINIT_MULTITHREADED));
			VerifyHResult(CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&_MMDeviceEnumerator));
			VerifyHResult(_MMDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &_MMDevice));
			VerifyHResult(XAudio2Create(&_XAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR));
			XAUDIO2_DEBUG_CONFIGURATION XAudio2DebugConfiguration = {};
			XAudio2DebugConfiguration.TraceMask			= XAUDIO2_LOG_ERRORS
														| XAUDIO2_LOG_WARNINGS
														| XAUDIO2_LOG_INFO
														| XAUDIO2_LOG_DETAIL
														| XAUDIO2_LOG_API_CALLS
														| XAUDIO2_LOG_FUNC_CALLS
														| XAUDIO2_LOG_TIMING
														| XAUDIO2_LOG_LOCKS
														| XAUDIO2_LOG_MEMORY
														| XAUDIO2_LOG_STREAMING;
			XAudio2DebugConfiguration.BreakMask			= XAUDIO2_LOG_ERRORS
														| XAUDIO2_LOG_WARNINGS;
			XAudio2DebugConfiguration.LogThreadID		= TRUE;
			XAudio2DebugConfiguration.LogFileline		= TRUE;
			XAudio2DebugConfiguration.LogFunctionName	= TRUE;
			XAudio2DebugConfiguration.LogTiming			= TRUE;
			_XAudio2->SetDebugConfiguration(&XAudio2DebugConfiguration);
			VerifyHResult(_MMDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, reinterpret_cast<void**>(&_AudioClient)));
			WAVEFORMATEX* MixFormat = nullptr;
			VerifyHResult(_AudioClient->GetMixFormat(&MixFormat));
			LPWSTR MMDeviceId = nullptr;
			VerifyHResult(_MMDevice->GetId(&MMDeviceId));
			VerifyHResult(_XAudio2->CreateMasteringVoice(&_XAudio2MasteringVoice, MixFormat->nChannels, MixFormat->nSamplesPerSec, 0, MMDeviceId));

			_XAudio2MasteringVoice->SetVolume(1);

			for (uint32_t VoiceIndex = 0; VoiceIndex < _SourceVoicesPool.size(); ++VoiceIndex)
			{
				MixFormat->wFormatTag = 1;
				MixFormat->nSamplesPerSec = 44100;
				MixFormat->nAvgBytesPerSec = 176400;
				MixFormat->nBlockAlign = 4;
				MixFormat->wBitsPerSample = 16;
				MixFormat->cbSize = 0;

				VerifyHResult(
					_XAudio2->CreateSourceVoice(
						&_SourceVoicesPool[VoiceIndex],
						MixFormat,
						0,
						XAUDIO2_DEFAULT_FREQ_RATIO,
						&_SourceVoiceCallbacksPool[VoiceIndex]
					)
				);
			}
		}

		MicrosoftAudioSystem::~MicrosoftAudioSystem()
		{
			for (uint32_t VoiceIndex = 0; VoiceIndex < _SourceVoicesPool.size(); ++VoiceIndex)
			{
				_SourceVoicesPool[VoiceIndex]->DestroyVoice();
				_SourceVoicesPool[VoiceIndex] = nullptr;
			}

			_XAudio2MasteringVoice->DestroyVoice();
			_XAudio2->StopEngine();
			_XAudio2->Release();
			_MMDevice->Release();
			_MMDeviceEnumerator->Release();
			CoUninitialize();
		}

		void MicrosoftAudioSystem::UpdateAudioSystem()
		{
			for (uint32_t VoiceIndex = 0; VoiceIndex < _AudioHandleQueues.size(); ++VoiceIndex)
			{
				if (_AudioHandleQueues[VoiceIndex].Count() > 0)
				{
					AudioHandle* CurrentAudioHandle = _AudioHandleQueues[VoiceIndex].Head();
					_AudioHandleQueues[VoiceIndex].PopHead();

					_SourceVoicesPool[VoiceIndex]->SubmitSourceBuffer(&static_cast<MicrosoftAudioHandle*>(CurrentAudioHandle)->XAudio2Buffer);
					_SourceVoicesPool[VoiceIndex]->Start();
				}
			}
		}

		void MicrosoftAudioSystem::Play(_In_ AudioHandle* InHandle)
		{
			uint32_t BestQueueCount = AudioHandleQueueSize;
			uint32_t BestVoiceIndex = InvalidVoiceIndex;

			for (uint32_t VoiceIndex = 0; VoiceIndex < _AudioHandleQueues.size(); ++VoiceIndex)
			{
				if (_AudioHandleQueues[VoiceIndex].Count() < BestQueueCount)
				{
					BestQueueCount = _AudioHandleQueues[VoiceIndex].Count();
					BestVoiceIndex = VoiceIndex;
				}
			}

			if (BestVoiceIndex != InvalidVoiceIndex)
			{
				_AudioHandleQueues[BestVoiceIndex].Push(InHandle);
				auto AudiomHandleMapIterator = _AudioHandleMap.find(InHandle);
				if (AudiomHandleMapIterator == _AudioHandleMap.end())
					AudiomHandleMapIterator = _AudioHandleMap.emplace(InHandle, AudioHandleQueueReferences()).first;
				AudiomHandleMapIterator->second.Queues.push_back(BestVoiceIndex);
			}
#if ETERNAL_DEBUG
			else
			{
				LogWrite(LogWarning, LogAudio, "[MicrosoftAudioSystem::Play] Play spamming");
			}
#endif
		}

		void MicrosoftAudioSystem::Stop(_In_ AudioHandle* InHandle)
		{
		}
	}
}

#endif
