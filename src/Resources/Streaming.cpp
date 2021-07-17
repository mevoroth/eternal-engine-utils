#include "Resources/Streaming.hpp"

#include "Parallel/Mutex.hpp"
#include "Parallel/MutexFactory.hpp"
#include "Parallel/MutexAutoLock.hpp"
#include "File/FilePath.hpp"

namespace Eternal
{
	namespace Resources
	{
		Streaming::Streaming()
			: _QueueMutex(CreateMutex())
		{
		}

		Streaming::~Streaming()
		{
			DestroyMutex(_QueueMutex);
		}

		void Streaming::EnqueueRequest(_In_ const StreamingRequest& InRequest)
		{
			_Enqueued.push_back(InRequest);
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
			if (_QueueMutex->TryLock())
			{
				_StreamingIndex = 1 - _StreamingIndex;
				_QueueMutex->Unlock();
			}
		}

		void Streaming::CommitRequests()
		{
			ETERNAL_PROFILER(BASIC)();
			MutexAutoLock QueueLock(_QueueMutex);
			StreamingQueue& Queue = GetFinishedStreaming();
			Queue.PendingRequests.insert(Queue.PendingRequests.end(), _Enqueued.begin(), _Enqueued.end());
			_Enqueued.clear();
		}

		StreamingQueue::StreamingQueue()
		{
		}

		StreamingQueue::~StreamingQueue()
		{
		}

		LevelRequest::LevelRequest(_In_ const string& InPath)
			: StreamingRequest(InPath, FileType::FILE_TYPE_LEVELS)
		{
		}
	}
}
