#pragma once

#include <string>

namespace Eternal
{
	namespace Parallel
	{
		using namespace std;

		class AtomicS32;

		class Task
		{
		public:
			Task();
			virtual ~Task() {}

			virtual void DoExecute() = 0;

			void Execute();
			bool IsDone() const;
			void SetTaskName(const string& TaskName);
			const string& GetTaskName() const
			{
				return _TaskName;
			}

		private:
			string		_TaskName;
			AtomicS32*	_IsExecuting = nullptr;
		};
	}
}
