#include "Resources/Streaming.hpp"
#include "Resources/MeshLoader.hpp"
#include "Resources/TextureLoader.hpp"
#include "Parallel/Mutex.hpp"
#include "Parallel/MutexFactory.hpp"
#include "Parallel/MutexAutoLock.hpp"
#include "Parallel/ConditionVariable.hpp"
#include "Parallel/ConditionVariableFactory.hpp"
#include "File/FilePath.hpp"

namespace Eternal
{
	namespace Resources
	{
		static constexpr uint32_t QUEUE_INDICES[] =
		{
			~0u,
			static_cast<uint32_t>(AssetType::ASSET_TYPE_LEVEL),
			static_cast<uint32_t>(AssetType::ASSET_TYPE_TEXTURE),
			static_cast<uint32_t>(AssetType::ASSET_TYPE_MESH)
		};

		static const uint32_t ConvertFileTypeToQueueIndex(_In_ const FileType& InFileType)
		{
			return QUEUE_INDICES[static_cast<uint32_t>(InFileType)];
		}

		Streaming::Streaming(_In_ TextureFactory& InTextureFactory)
			: _QueueMutex(CreateMutex())
			, _SleepMutex(CreateMutex())
			, _SleepConditionVariable(CreateConditionVariable())
		{
			RegisterLoader(AssetType::ASSET_TYPE_MESH, new MeshLoader());
			RegisterLoader(AssetType::ASSET_TYPE_TEXTURE, new TextureLoader(InTextureFactory));
		}

		Streaming::~Streaming()
		{
			for (uint32_t LoaderIndex = 0; LoaderIndex < _Loaders.size(); ++LoaderIndex)
			{
				const_cast<StreamingLoader*>(_Loaders[LoaderIndex])->DestroyPayloadLoader();
				_Loaders[LoaderIndex] = nullptr;
			}

			DestroyConditionVariable(_SleepConditionVariable);
			DestroyMutex(_SleepMutex);
			DestroyMutex(_QueueMutex);
		}

		void Streaming::RegisterLoader(_In_ const AssetType& InAssetType, _In_ const StreamingLoader* InLoader)
		{
			_Loaders[static_cast<int32_t>(InAssetType)] = InLoader;
		}

		const StreamingLoader* Streaming::GetLoader(_In_ const AssetType& InAssetType)
		{
			return _Loaders[static_cast<int32_t>(InAssetType)];
		}

		void Streaming::EnqueueRequest(_In_ StreamingRequest* InRequest)
		{
			_Enqueued[ConvertFileTypeToQueueIndex(InRequest->RequestType)].push_back(InRequest);
		}

		StreamingQueue& Streaming::GetFinishedStreaming()
		{
			return _StreamingQueues[1 - _StreamingIndex];
		}

		StreamingQueue& Streaming::GetPendingStreaming()
		{
			return _StreamingQueues[_StreamingIndex];
		}

		void Streaming::AdvanceStreaming()
		{
			ETERNAL_PROFILER(BASIC)();
			bool ShouldRotateBuffers = false;
			while (!ShouldRotateBuffers && _Running)
			{
				if (_QueueMutex->TryLock())
				{
					RequestQueueType& NextPendingRequests = GetFinishedStreaming().PendingRequests;
					PayloadQueueType& CurrentLoadedPayloads = GetPendingStreaming().LoadedRequests;

					for (uint32_t QueueType = 0; QueueType < NextPendingRequests.size(); ++QueueType)
					{
						if (NextPendingRequests[QueueType].size() > 0 ||
							CurrentLoadedPayloads[QueueType].size() > 0)
						{
							ShouldRotateBuffers = true;
							break;
						}
					}

					if (ShouldRotateBuffers)
						_StreamingIndex = 1 - _StreamingIndex;
					
					_QueueMutex->Unlock();
				}

				_SleepMutex->Lock();
				_SleepConditionVariable->WaitFor(*_SleepMutex, 1000);
				_SleepMutex->Unlock();
			}
		}

		void Streaming::GatherPayloads()
		{
			ETERNAL_PROFILER(BASIC)();
			MutexAutoLock QueueLock(_QueueMutex);
			StreamingQueue& Queue = GetFinishedStreaming();
			for (uint32_t QueueType = 0; QueueType < _Gathered.size(); ++QueueType)
			{
				_Gathered[QueueType].insert(
					_Gathered[QueueType].end(),
					Queue.LoadedRequests[QueueType].begin(),
					Queue.LoadedRequests[QueueType].end()
				);
				Queue.LoadedRequests[QueueType].clear();

				//for (uint32_t RequestIndex = 0; RequestIndex < Queue.PendingDestroyedRequests[QueueType].size(); ++RequestIndex)
				//{
				//	delete Queue.PendingDestroyedRequests[QueueType][RequestIndex];
				//	Queue.PendingDestroyedRequests[QueueType][RequestIndex] = nullptr;
				//}
				//Queue.PendingDestroyedRequests[QueueType].clear();
			}
		}

		void Streaming::CommitRequests()
		{
			ETERNAL_PROFILER(BASIC)();
			{
				MutexAutoLock QueueLock(_QueueMutex);
				StreamingQueue& Queue = GetFinishedStreaming();
				for (uint32_t QueueType = 0; QueueType < Queue.PendingRequests.size(); ++QueueType)
				{
					Queue.PendingRequests[QueueType].insert(
						Queue.PendingRequests[QueueType].end(),
						_Enqueued[QueueType].begin(),
						_Enqueued[QueueType].end()
					);
					_Enqueued[QueueType].clear();

					//std::swap(Queue.PendingDestroyedRequests[QueueType], _Destroyed[QueueType]);
				}
			}
			_SleepMutex->Unlock();
			_SleepConditionVariable->NotifyOne();
		}

		StreamingQueue::StreamingQueue()
		{
		}

		StreamingQueue::~StreamingQueue()
		{
		}

		void Payload::AddRequest(_In_ StreamingRequest* InRequest)
		{
			AdditionalRequests[ConvertFileTypeToQueueIndex(InRequest->RequestType)].push_back(InRequest);
		}
	}
}
