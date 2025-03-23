#include "Resources/Streaming.hpp"
#include "Resources/MeshLoader.hpp"
#include "Resources/TextureLoader.hpp"
#include "Parallel/Mutex.hpp"
#include "Parallel/MutexFactory.hpp"
#include "Parallel/MutexAutoLock.hpp"
#include "Parallel/ConditionVariable.hpp"
#include "Parallel/ConditionVariableFactory.hpp"
#include "Import/mat/ImportMat.hpp"
#include "File/FilePath.hpp"

#include "Resources/Payload.hpp"

namespace Eternal
{
	namespace Resources
	{
		static constexpr uint32_t QUEUE_INDICES[] =
		{
			~0u,
			~0u,
			static_cast<uint32_t>(AssetType::ASSET_TYPE_LEVEL),
			static_cast<uint32_t>(AssetType::ASSET_TYPE_TEXTURE),
			static_cast<uint32_t>(AssetType::ASSET_TYPE_MESH),
			~0u,
			~0u,
			~0u,
			~0u,
			~0u,
			~0u
		};

		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(QUEUE_INDICES) == static_cast<uint32_t>(FileType::FILE_TYPE_COUNT), "Mismatch between QUEUE_INDICES and FileType");

		static const uint32_t ConvertFileTypeToQueueIndex(_In_ const FileType& InFileType)
		{
			return QUEUE_INDICES[static_cast<uint32_t>(InFileType)];
		}

		template<typename QueueType>
		void MergeStreamingQueues(_In_ QueueType& InOutQueue, _Inout_ QueueType& InOutQueueToMerge)
		{
			for (uint32_t QueueTypeIndex = 0; QueueTypeIndex < InOutQueue.size(); ++QueueTypeIndex)
			{
				InOutQueue[QueueTypeIndex].insert(
					InOutQueue[QueueTypeIndex].end(),
					InOutQueueToMerge[QueueTypeIndex].begin(),
					InOutQueueToMerge[QueueTypeIndex].end()
				);
				InOutQueueToMerge[QueueTypeIndex].clear();
			}
		}

		void MergeMaterialUpdateBatches(_Inout_ MaterialUpdateBatcher& InOutMaterialUpdateBatches, _Inout_ MaterialUpdateBatcher& InOutMaterialUpdateBatchesToMerge)
		{
			unordered_map<TextureKey, MaterialUpdateBatch>& MaterialUpdatesToMerge	= InOutMaterialUpdateBatchesToMerge.MaterialUpdates;
			unordered_map<TextureKey, MaterialUpdateBatch>& MaterialUpdates			= InOutMaterialUpdateBatches.MaterialUpdates;
			for (auto MaterialUpdateToMergeIterator = MaterialUpdatesToMerge.begin(); MaterialUpdateToMergeIterator != MaterialUpdatesToMerge.end(); ++MaterialUpdateToMergeIterator)
			{
				auto MaterialUpdateIterator = MaterialUpdates.find(MaterialUpdateToMergeIterator->first);
				if (MaterialUpdateIterator == MaterialUpdates.end())
					MaterialUpdateIterator = MaterialUpdates.insert({ MaterialUpdateToMergeIterator->first, MaterialUpdateBatch() }).first;

				MaterialUpdateIterator->second.Materials.insert(
					MaterialUpdateIterator->second.Materials.end(),
					MaterialUpdateToMergeIterator->second.Materials.begin(),
					MaterialUpdateToMergeIterator->second.Materials.end()
				);
				MaterialUpdateToMergeIterator->second.Materials.clear();

				if (!MaterialUpdateIterator->second.TextureData.TextureData)
				{
					MaterialUpdateIterator->second.TextureData		= MaterialUpdateToMergeIterator->second.TextureData;
				}
				MaterialUpdateToMergeIterator->second.TextureData	= RawTextureData();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Payload
		//Payload::~Payload()
		//{
		//	ETERNAL_ASSERT(IsProcessed());
		//}

		//////////////////////////////////////////////////////////////////////////
		// StreamingInternal
		struct StreamingInternal
		{
			MaterialUpdateBatcher MaterialUpdateBatcher_StreamingThread;
			MaterialUpdateBatcher MaterialUpdateBatcher_MainThread;
		};

		//////////////////////////////////////////////////////////////////////////
		// Streaming
		Streaming::Streaming(_In_ TextureFactory& InTextureFactory)
			: _QueueMutex(CreateMutex())
			, _SleepMutex(CreateMutex())
			, _SleepConditionVariable(CreateConditionVariable())
			, _StreamingInternal(new StreamingInternal())
			, _ImportMat(new ImportMat())
		{
			RegisterLoader(AssetType::ASSET_TYPE_MESH, new MeshLoader(_ImportMat));
			RegisterLoader(AssetType::ASSET_TYPE_TEXTURE, new TextureLoader(InTextureFactory, _StreamingInternal->MaterialUpdateBatcher_StreamingThread));
		}

		Streaming::~Streaming()
		{
			for (uint32_t LoaderIndex = 0; LoaderIndex < _Loaders.size(); ++LoaderIndex)
			{
				const_cast<StreamingLoader*>(_Loaders[LoaderIndex])->DestroyPayloadLoader();
				_Loaders[LoaderIndex] = nullptr;
			}

			delete _StreamingInternal;
			_StreamingInternal = nullptr;

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

		void Streaming::EnqueueRequest_MainThread(_In_ StreamingRequest* InRequest)
		{
			_Enqueued[ConvertFileTypeToQueueIndex(InRequest->RequestType)].push_back(InRequest);
		}

		StreamingQueue& Streaming::GetFinishedStreaming_MainThread()
		{
			return _StreamingQueues[1 - _StreamingIndex];
		}

		StreamingQueue& Streaming::GetPendingStreaming_StreamingThread()
		{
			return _StreamingQueues[_StreamingIndex];
		}

		RequestQueueType& Streaming::GetPending()
		{
			return _Pending;
		}

		void Streaming::AdvanceStreaming_StreamingThread()
		{
			ETERNAL_PROFILER(BASIC)();
			bool ShouldRotateBuffers = false;
			bool ShouldTickStreaming = false;
			while (!(ShouldRotateBuffers || ShouldTickStreaming) && _Running)
			{
				if (_QueueMutex->TryLock())
				{
					RequestQueueType& NextPendingRequests = GetFinishedStreaming_MainThread().PendingRequests;
					PayloadQueueType& CurrentLoadedPayloads = GetPendingStreaming_StreamingThread().LoadedRequests;

					for (uint32_t QueueType = 0; QueueType < NextPendingRequests.size(); ++QueueType)
					{
						if (NextPendingRequests[QueueType].size() > 0 ||
							CurrentLoadedPayloads[QueueType].size() > 0)
						{
							ShouldRotateBuffers = true;
							break;
						}
						if (_Pending[QueueType].size() > 0)
						{
							ShouldTickStreaming = true;
							break;
						}
					}

					if (ShouldRotateBuffers || ShouldTickStreaming)
						MergeMaterialUpdateBatches(_StreamingInternal->MaterialUpdateBatcher_MainThread, _StreamingInternal->MaterialUpdateBatcher_StreamingThread);

					if (ShouldRotateBuffers)
						_StreamingIndex = 1 - _StreamingIndex;
					
					_QueueMutex->Unlock();
				}

				_SleepMutex->Lock();
				_SleepConditionVariable->WaitFor(*_SleepMutex, 1000);
				_SleepMutex->Unlock();
			}
		}

		void Streaming::GatherPayloads_MainThread(_Inout_ MaterialUpdateBatcher& InOutMaterialUpdateBatches)
		{
			ETERNAL_PROFILER(BASIC)();
			MutexAutoLock QueueLock(_QueueMutex);
			StreamingQueue& Queue = GetFinishedStreaming_MainThread();
			MergeStreamingQueues(_Gathered, Queue.LoadedRequests);
			MergeMaterialUpdateBatches(InOutMaterialUpdateBatches, _StreamingInternal->MaterialUpdateBatcher_MainThread);
		}

		void Streaming::CommitEnqueued_StreamingThread()
		{
			ETERNAL_PROFILER(BASIC)();
			RequestQueueType& StreamingRequests = GetPendingStreaming_StreamingThread().PendingRequests;
			MergeStreamingQueues(_Pending, StreamingRequests);
		}

		void Streaming::ProcessPending_StreamingThread()
		{
			RequestQueueType& StreamingRequests = GetPending();
			PayloadQueueType& StreamingPayloads = GetPendingStreaming_StreamingThread().LoadedRequests;

			uint32_t TotalProcessedRequestsCount = 0u;
			_ProcessQueues_StreamingThread(StreamingRequests, StreamingPayloads, TotalProcessedRequestsCount);
		}

		void Streaming::CommitRequests_MainThread()
		{
			ETERNAL_PROFILER(BASIC)();
			{
				MutexAutoLock QueueLock(_QueueMutex);
				StreamingQueue& Queue = GetFinishedStreaming_MainThread();
				MergeStreamingQueues(Queue.PendingRequests, _Enqueued);
			}
			_SleepMutex->Lock();
			_SleepConditionVariable->NotifyOne();
			_SleepMutex->Unlock();
		}

		StreamingQueue::StreamingQueue()
		{
			for (uint32_t QueueType = 0; QueueType < static_cast<uint32_t>(AssetType::ASSET_TYPE_COUNT); ++QueueType)
			{
				PendingRequests[QueueType].reserve(Streaming::StreamingEstimatedPendingRequestsCount);
				LoadedRequests[QueueType].reserve(Streaming::StreamingEstimatedLoadedRequestsCount);
				PendingDestroyedRequests[QueueType].reserve(Streaming::StreamingEstimatedPendingDestroyedRequestsCount);
			}
		}

		StreamingQueue::~StreamingQueue()
		{
		}

		bool Streaming::_ProcessQueues_StreamingThread(_In_ RequestQueueType& InStreamingRequests, _Inout_ PayloadQueueType& InOutStreamingPayloads, _Inout_ uint32_t& InOutTotalProcessedRequestsCount)
		{
			uint32_t EmptyQueuesCount = 0u;

			for (uint32_t QueueType = 0; InOutTotalProcessedRequestsCount < Streaming::StreamingBudgetPerFrame && QueueType < InStreamingRequests.size(); ++QueueType)
			{
				vector<StreamingRequest*>& Requests = InStreamingRequests[QueueType];

				const StreamingLoader* Loader = GetLoader(static_cast<AssetType>(QueueType));

				uint32_t QueueProcessedRequestsCount = 0u;

				while (InOutTotalProcessedRequestsCount < Streaming::StreamingBudgetPerFrame && QueueProcessedRequestsCount < Requests.size())
				{
					InOutStreamingPayloads[QueueType].push_back(nullptr);
					Payload*& LastPayload = InOutStreamingPayloads[QueueType].back();
					Loader->LoadPayload(Requests[0], LastPayload);

					ETERNAL_ASSERT(LastPayload);
					if (!_ProcessQueues_StreamingThread(LastPayload->AdditionalRequests, InOutStreamingPayloads, InOutTotalProcessedRequestsCount))
					{
						Requests.erase(Requests.end() - QueueProcessedRequestsCount, Requests.end());
						QueueProcessedRequestsCount = 0u;
						MergeStreamingQueues(InStreamingRequests, LastPayload->AdditionalRequests);
					}

					delete Requests[0];
					Requests[0] = nullptr;

					std::swap(Requests[0], Requests[Requests.size() - QueueProcessedRequestsCount - 1]);

					++QueueProcessedRequestsCount;
					++InOutTotalProcessedRequestsCount;
				}

				Requests.erase(Requests.end() - QueueProcessedRequestsCount, Requests.end());

				EmptyQueuesCount += Requests.empty();
			}

			return EmptyQueuesCount == InStreamingRequests.size();
		}

		//////////////////////////////////////////////////////////////////////////
		// Payload
		void Payload::AddRequest(_In_ StreamingRequest* InRequest)
		{
			AdditionalRequests[ConvertFileTypeToQueueIndex(InRequest->RequestType)].push_back(InRequest);
		}
	}
}
