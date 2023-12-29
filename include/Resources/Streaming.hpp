#pragma once

#include <array>
#include <string>
#include <vector>

namespace Eternal
{
	namespace Parallel
	{
		class Mutex;
		class ConditionVariable;
	}
	namespace FileSystem
	{
		enum class FileType;
	}
	namespace Core
	{
		class MeshComponent;
	}
	namespace Import
	{
		class ImportMat;
	}

	namespace Resources
	{
		using namespace std;
		using namespace Eternal::Parallel;
		using namespace Eternal::FileSystem;
		using namespace Eternal::Core;
		using namespace Eternal::Import;

		enum class AssetType
		{
			ASSET_TYPE_LEVEL = 0,
			ASSET_TYPE_MESH,
			ASSET_TYPE_TEXTURE,
			ASSET_TYPE_COUNT
		};

		struct StreamingRequest;
		struct Payload;
		class TextureFactory;
		struct MaterialUpdateBatcher;
		struct StreamingInternal;

		struct StreamingLoader
		{
			virtual void LoadPayload(_In_ const StreamingRequest* InRequest, _Out_ Payload*& OutPayload) const = 0;
			virtual void DestroyPayloadLoader() = 0;
		};

		struct StreamingRequest
		{
			string RequestPath;
			FileType RequestType;

			virtual ~StreamingRequest() {}

		protected:
			StreamingRequest(_In_ const string& InPath, _In_ const FileType& InFileType)
				: RequestPath(InPath)
				, RequestType(InFileType)
			{
				ETERNAL_ASSERT(InPath.size() > 0);
			}
		};

		using RequestQueueType = array<vector<StreamingRequest*>, static_cast<int32_t>(AssetType::ASSET_TYPE_COUNT)>;
		using PayloadQueueType = array<vector<Payload*>, static_cast<int32_t>(AssetType::ASSET_TYPE_COUNT)>;

		template<typename QueueType>
		void MergeStreamingQueues(_Inout_ QueueType& InOutQueue, _Inout_ QueueType& InQueueToMerge);
		void MergeMaterialUpdateBatches(_Inout_ MaterialUpdateBatcher& InOutMaterialUpdateBatches, _Inout_ MaterialUpdateBatcher& InOutMaterialUpdateBatchesToMerge);

		struct Payload
		{
			virtual ~Payload() { ETERNAL_ASSERT(IsProcessed()); }

			void AddRequest(_In_ StreamingRequest* InRequest);
			
			template<typename PayloadFunction>
			void Process(_In_ PayloadFunction PayloadFunctor)
			{
				PayloadFunctor(*this);
			}

			void MarkProcessed()
			{
				Processed = true;
			}

			bool IsProcessed() const
			{
				return Processed;
			}

			RequestQueueType AdditionalRequests;
			bool Processed = false;
		};

		struct StreamingQueue
		{
			StreamingQueue();
			~StreamingQueue();

			RequestQueueType PendingRequests;
			PayloadQueueType LoadedRequests;
			PayloadQueueType PendingDestroyedRequests;
		};

		class Streaming
		{
		public:
			static constexpr uint32_t StreamingQueueCount								= 2;
			static constexpr uint32_t StreamingBudgetPerFrame							= 4;
			static constexpr uint32_t StreamingEstimatedPendingRequestsCount			= 8192;
			static constexpr uint32_t StreamingEstimatedLoadedRequestsCount				= 64;
			static constexpr uint32_t StreamingEstimatedPendingDestroyedRequestsCount	= 128;

			Streaming(_In_ TextureFactory& InTextureFactory);
			~Streaming();

			void RegisterLoader(_In_ const AssetType& InAssetType, _In_ const StreamingLoader* InLoader);
			const StreamingLoader* GetLoader(_In_ const AssetType& InAssetType);

			void EnqueueRequest_MainThread(_In_ StreamingRequest* InRequest);
			void CommitEnqueued_StreamingThread();
			void ProcessPending_StreamingThread();
			void CommitRequests_MainThread();
			void GatherPayloads_MainThread(_Inout_ MaterialUpdateBatcher& InOutMaterialUpdateBatches);
			template<typename PayloadFunction>
			void ProcessGathered_MainThread(_In_ const AssetType& InAssetType, _Inout_ PayloadQueueType& InOutDelayedDestroyed, _In_ PayloadFunction InPayloadFunctor)
			{
				vector<Payload*>& Payloads = _Gathered[static_cast<int32_t>(InAssetType)];
				for (uint32_t PayloadIndex = 0; PayloadIndex < Payloads.size(); ++PayloadIndex)
					Payloads[PayloadIndex]->Process(InPayloadFunctor);
				std::swap(InOutDelayedDestroyed[static_cast<int32_t>(InAssetType)], Payloads);
			}
			StreamingQueue& GetFinishedStreaming_MainThread();
			StreamingQueue& GetPendingStreaming_StreamingThread();
			RequestQueueType& GetPending();
			void AdvanceStreaming_StreamingThread();
			void Shutdown() { _Running = false; }

		private:

			bool _ProcessQueues_StreamingThread(_In_ RequestQueueType& InStreamingRequests, _Inout_ PayloadQueueType& InOutStreamingPayloads, _Inout_ uint32_t& InOutTotalProcessedRequestsCount);

			array<const StreamingLoader*, static_cast<int32_t>(AssetType::ASSET_TYPE_COUNT)> _Loaders;

			RequestQueueType							_Enqueued;
			RequestQueueType							_Pending;
			PayloadQueueType							_Gathered;
			array<StreamingQueue, StreamingQueueCount>	_StreamingQueues;
			uint32_t									_StreamingIndex			= 0;
			Mutex*										_QueueMutex				= nullptr;
			Mutex*										_SleepMutex				= nullptr;
			ConditionVariable*							_SleepConditionVariable	= nullptr;
			StreamingInternal*							_StreamingInternal		= nullptr;
			ImportMat*									_ImportMat				= nullptr;
			bool										_Running				= true;
		};
	}
}
