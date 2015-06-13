#ifndef _TASK_HPP_
#define _TASK_HPP_

namespace Eternal
{
	namespace Parallel
	{
		class Task
		{
		public:
			void SetFinished();
			bool IsFinished() const;
			virtual void DoTask() = 0;

		private:
			bool _Finished = false;
		};
	}
}

#endif
