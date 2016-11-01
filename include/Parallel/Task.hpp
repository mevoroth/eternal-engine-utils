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

			void Setup();
			void Reset();
			void Execute();

			virtual void DoSetup() = 0;
			virtual void DoReset() = 0;
			virtual void DoExecute() = 0;
			void Schedule();
			void SetFrameConstraint(_In_ bool FrameConstraint);
			bool GetFrameConstraint() const;
			void SetInstanceCount(_In_ int InstanceCount);
			int GetInstanceCount() const;

			const TaskState& GetState() const
			{
				return _TaskState;
			}

			void SetTaskName(const string& TaskName);
			const string& GetTaskName() const
			{
#ifdef ETERNAL_DEBUG
				return _TaskName;
#else
				return "";
#endif
			}

		protected:
			inline void SetState(const TaskState& State)
			{
				_TaskState = State;
			}

		private:
			TaskState _TaskState = IDLE;
			bool _FrameConstraint = true;
			int _InstanceCount = 1;
			int _RemainingInstanceCount = 1;
#ifdef ETERNAL_DEBUG
			string _TaskName;
#endif
		};
	}
}

#endif
