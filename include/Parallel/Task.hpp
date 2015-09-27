#ifndef _TASK_HPP_
#define _TASK_HPP_

#include "Macros/Macros.hpp"
#include "Parallel/AtomicInt.hpp"

namespace Eternal
{
	namespace Parallel
	{
		class Task
		{
		public:
			Task();
			virtual ~Task();
			void SetFinished();
			bool IsFinished() const;
			virtual void DoTask() = 0;
			inline void AddRef()
			{
				_RefCount->Add();
			}
			inline void Release()
			{
				ETERNAL_ASSERT(_RefCount->Load() > 0);
				_RefCount->Sub();
			}
			inline bool IsNotReferenced() const
			{
				return _RefCount->Load() == 0;
			}

		private:
			bool _Finished = false;
			AtomicInt* _RefCount = nullptr;
		};
	}
}

#endif
