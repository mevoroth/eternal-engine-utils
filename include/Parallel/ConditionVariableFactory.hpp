#pragma once

namespace Eternal
{
	namespace Parallel
	{
		class ConditionVariable;
		
		ConditionVariable* CreateConditionVariable();
		void DestroyConditionVariable(_Inout_ ConditionVariable*& InOutConditionVariable);
	}
}
