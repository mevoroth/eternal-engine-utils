#ifndef _TASK_HPP_
#define _TASK_HPP_

#ifdef ETERNAL_DEBUG
#include <string>
using namespace std;
#endif

namespace Eternal
{
	namespace Parallel
	{
		using namespace std;

		class Task
		{
		public:
			enum TaskState
			{
				IDLE,
				SCHEDULED,
				SETTINGUP,
				SETUP,
				EXECUTING,
				DONE
			};
			virtual ~Task() {}

			virtual void Setup() = 0;
			virtual void Reset() = 0;
			virtual void Execute() = 0;
			void Schedule();

			const TaskState& GetState() const
			{
				return _TaskState;
			}

			void SetTaskName(const string& TaskName);

		protected:
			inline void SetState(const TaskState& State)
			{
				_TaskState = State;
			}

		private:
			TaskState _TaskState = IDLE;
			
#ifdef ETERNAL_DEBUG
			string _TaskName;
#endif
		};
	}
}

#endif
