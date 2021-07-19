#pragma once

#include <string>

namespace Eternal
{
	namespace Core
	{
		class System;
	}
	namespace Parallel
	{
		using namespace std;
		using namespace Eternal::Core;

		class AtomicS32;

		struct TaskCreateInformation
		{
			TaskCreateInformation(_In_ System& InSystem, _In_ const string& InName)
				: SystemContext(InSystem)
				, Name(InName)
			{
			}

			System& SystemContext;
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
			System& GetSystem() { return _TaskCreateInformation.SystemContext; }

		private:
			TaskCreateInformation	_TaskCreateInformation;
			AtomicS32*				_IsExecuting = nullptr;
		};
	}
}
