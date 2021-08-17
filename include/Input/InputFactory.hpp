#pragma once

#include <vector>

namespace Eternal
{
	namespace InputSystem
	{
		class Input;

		enum class InputType
		{
			INPUT_TYPE_WIN,
			INPUT_TYPE_XINPUT,
			INPUT_TYPE_MULTI
		};

		Input* CreateInput(_In_ const InputType& InInputType);
		Input* CreateMultiInput(_In_ const std::vector<InputType>& InInputTypes);
		void DestroyInput(_Inout_ Input*& InOutInput);
	}
}
