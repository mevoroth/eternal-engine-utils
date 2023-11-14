#pragma once

#include <vector>

namespace Eternal
{
	namespace InputSystem
	{
		class Input;

		enum class InputType
		{
			INPUT_TYPE_KEYBOARD,
			INPUT_TYPE_XINPUT,
			INPUT_TYPE_SCE_PAD,
			INPUT_TYPE_MULTI
#if ETERNAL_USE_PRIVATE
			#include "Input/InputFactoryInputTypePrivate.hpp"
#else
			, INPUT_TYPE_DEFAULT_PAD = INPUT_TYPE_XINPUT
#endif
		};

		Input* CreateInput(_In_ const InputType& InInputType);
		Input* CreateMultiInput(_In_ const std::vector<InputType>& InInputTypes);
		void DestroyInput(_Inout_ Input*& InOutInput);
	}
}
