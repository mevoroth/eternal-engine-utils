#pragma once

#include <string>

namespace Eternal
{
	namespace Parallel
	{
		using namespace std;

		class AtomicS32;

		struct TaskCreateInformation
		{
			TaskCreateInformation(_In_ const string& InName)
				: Name(InName)
			{
			}

			string Name;
		};

		class Task
		{
		public:
			Task(_In_ const TaskCreateInformation& InTaskCreateInformation);
			virtual ~Task() {}

			virtual void DoExecute() = 0;

			void Execute();
			bool IsDone() const;
			inline const string& GetTaskName() const { return _TaskCreateInformation.Name; }

		protected:

		private:
			TaskCreateInformation	_TaskCreateInformation;
			AtomicS32*				_IsExecuting = nullptr;
		};
	}
}
