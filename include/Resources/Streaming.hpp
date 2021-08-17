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

	namespace Resources
	{
		using namespace std;
		using namespace Eternal::Parallel;
		using namespace Eternal::FileSystem;
		using namespace Eternal::Core;

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

		struct StreamingLoader
		{
			virtual void LoadPayload(_In_ const StreamingRequest* InRequest, _Out_ Payload*& OutPayload) const = 0;
			virtual void DestroyPayloadLoader() = 0;
		};

		struct StreamingRequest
		{
			string RequestPath;
			FileType RequestType;

		protected:
			StreamingRequest(_In_ const string& InPath, _In_ const FileType& InFileType)
				: RequestPath(InPath)
				, RequestType(InFileType)
			{
			}
		};

		using RequestQueueType = array<vector<StreamingRequest*>, static_cast<int32_t>(AssetType::ASSET_TYPE_COUNT)>;
		using PayloadQueueType = array<vector<Payload*>, static_cast<int32_t>(AssetType::ASSET_TYPE_COUNT)>;

		struct Payload
		{
			void AddRequest(_In_ StreamingRequest* InRequest);
			
			template<typename PayloadFunctor>
			void Process(_In_ PayloadFunctor PayloadFunction)
			{
				PayloadFunction(*this);
			}

			RequestQueueType AdditionalRequests;
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
			static constexpr uint32_t StreamingQueueCount = 2;

			Streaming(_In_ TextureFactory& InTextureFactory);
			~Streaming();

			void RegisterLoader(_In_ const AssetType& InAssetType, _In_ const StreamingLoader* InLoader);
			const StreamingLoader* GetLoader(_In_ const AssetType& InAssetType);

			void EnqueueRequest(_In_ StreamingRequest* InRequest);
			void CommitRequests();
			void GatherPayloads();
			template<typename PayloadFunctor>
			void ProcessGathered(_In_ const AssetType& InAssetType, _Inout_ PayloadQueueType& InOutDelayedDestroyed, _In_ PayloadFunctor InPayloadFunction)
			{
				vector<Payload*>& Payloads = _Gathered[static_cast<int32_t>(InAssetType)];
				for (uint32_t PayloadIndex = 0; PayloadIndex < Payloads.size(); ++PayloadIndex)
					Payloads[PayloadIndex]->Process(InPayloadFunction);
				std::swap(InOutDelayedDestroyed[static_cast<int32_t>(InAssetType)], Payloads);
			}
			StreamingQueue& GetFinishedStreaming();
			StreamingQueue& GetPendingStreaming();
			void AdvanceStreaming();
			void Shutdown() { _Running = false; }

		private:

			array<const StreamingLoader*, static_cast<int32_t>(AssetType::ASSET_TYPE_COUNT)> _Loaders;

			RequestQueueType							_Enqueued;
			PayloadQueueType							_Gathered;
			array<StreamingQueue, StreamingQueueCount>	_StreamingQueues;
			uint32_t									_StreamingIndex			= 0;
			Mutex*										_QueueMutex				= nullptr;
			Mutex*										_SleepMutex				= nullptr;
			ConditionVariable*							_SleepConditionVariable	= nullptr;
			bool										_Running				= true;
		};
	}
}
