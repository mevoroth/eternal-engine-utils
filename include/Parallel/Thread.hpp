#pragma once

#include <string>

namespace Eternal
{
	namespace Parallel
	{
		using namespace std;

		using ThreadFunction = uint32_t(*)(void*);

		class Thread
		{
		public:

			Thread(_In_ const string& InName)
				: Name(InName)
			{
			}
			virtual ~Thread() {}
			virtual void Create(_In_ ThreadFunction InFunction, _In_ void* InFunctionParameters) = 0;

			const string& GetName() const { return Name; }

		private:
			string Name;
		};
	}
}
