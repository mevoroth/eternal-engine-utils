#ifndef _TASK_HPP_
#define _TASK_HPP_

#include <vector>

namespace Eternal
{
	namespace Parallel
	{
		using namespace std;

		class Task
		{
		public:
			Task();
			virtual ~Task();

			void DependsOn(Task* TaskObj);
			bool TaskIsExecutable();
			virtual bool TaskIsExecuted() = 0;
			virtual void Setup() = 0;
			virtual void Execute() = 0;

		private:
			vector<Task*> _Dependencies;
		};
	}
}

#endif
