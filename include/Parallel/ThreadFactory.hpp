#pragma once

#include <string>

namespace Eternal
{
	namespace Parallel
	{
		using namespace std;

		using ThreadFunction = uint32_t(*)(void*);
		class Thread;

		Thread* CreateThread(_In_ const string& InName, _In_ ThreadFunction InFunction, _In_ void* InFunctionParameters);
		void DestroyThread(_Inout_ Thread*& InOutThread);
	}
}
