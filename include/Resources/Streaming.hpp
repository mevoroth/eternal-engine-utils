#pragma once

#include <array>
#include <string>
#include <vector>

namespace Eternal
{
	namespace Parallel
	{
		class Mutex;
	}
	namespace FileSystem
	{
		enum class FileType;
	}

	namespace Resources
	{
		using namespace std;
		using namespace Eternal::Parallel;
		using namespace Eternal::FileSystem;

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

		struct StreamingQueue
		{
			StreamingQueue();
			~StreamingQueue();

			vector<StreamingRequest> PendingRequests;
		};

		struct LevelRequest : public StreamingRequest
		{
			LevelRequest(_In_ const string& InPath);
		};

		class Streaming
		{
		public:
			static constexpr uint32_t StreamingQueueCount = 2;

			Streaming();
			~Streaming();

			void EnqueueRequest(_In_ const StreamingRequest& InRequest);
			void CommitRequests();
			StreamingQueue& GetFinishedStreaming();
			StreamingQueue& GetPendingStreaming();
			void AdvanceStreaming();

		private:
			vector<StreamingRequest>									_Enqueued;
			array<StreamingQueue, StreamingQueueCount>					_StreamingQueues;
			uint32_t													_StreamingIndex	= 0;
			Mutex*														_QueueMutex		= nullptr;
		};
	}
}
