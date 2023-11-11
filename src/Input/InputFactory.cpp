#include "Input/InputFactory.hpp"

#include "Log/Log.hpp"
#include "Input/KeyboardInput/KeyboardInput.hpp"
#include "Input/XInput/XInput.hpp"
#include "Input/MultiInput/MultiInput.hpp"

namespace Eternal
{
	namespace InputSystem
	{
		using namespace Eternal::LogSystem;

		Input* CreateInput(_In_ const InputType& InInputType)
		{
			switch (InInputType)
			{
			case InputType::INPUT_TYPE_KEYBOARD:
				LogWrite(LogInfo, LogEngine, "[Input::CreateInput]Creating Keyboard input");
				return new KeyboardInput();

			#if ETERNAL_PLATFORM_WINDOWS
			case InputType::INPUT_TYPE_XINPUT:
				LogWrite(LogInfo, LogEngine, "[Input::CreateInput]Creating Xbox Pad input");
				return new XInput();
			#endif

			case InputType::INPUT_TYPE_MULTI:	// Must be created with CreateMultiInput
			default:
				ETERNAL_BREAK();
				break;
			}
			return nullptr;
		}

		Input* CreateMultiInput(_In_ const std::vector<InputType>& InInputTypes)
		{
			LogWrite(LogInfo, LogEngine, "[Input::CreateMultiInput]Creating Multi input");
			MultiInput* OutMultiInput = new MultiInput();
			for (uint32_t InputIndex = 0; InputIndex < InInputTypes.size(); ++InputIndex)
				OutMultiInput->Add(CreateInput(InInputTypes[InputIndex]));
			return OutMultiInput;
		}

		void DestroyInput(_Inout_ Input*& InOutInput)
		{
			delete InOutInput;
			InOutInput = nullptr;
		}
	}
}
