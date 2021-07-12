#pragma once

#include "Parallel/Thread.hpp"

#include <thread>

using namespace std;

namespace Eternal
{
	namespace Parallel
	{
		class StdThread final : public Thread
		{
		public:
			StdThread(_In_ const string& InName)
				: Thread(InName)
			{
			}

			virtual void Create(_In_ ThreadFunction InFunction, _In_ void* InFunctionParameters) override final;

		private:
			thread _Thread;
		};
	}
}
