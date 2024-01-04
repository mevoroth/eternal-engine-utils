#include "Input/InputFactory.hpp"

#include "Log/Log.hpp"
#include "Input/KeyboardInput/KeyboardInput.hpp"
#include "Input/MouseInput/MouseInput.hpp"
#include "Input/XInput/XInput.hpp"
#include "Input/MultiInput/MultiInput.hpp"

#if ETERNAL_USE_PRIVATE
#include "Input/ScePadInput/ScePadInputFactoryPrivate.hpp"
#include "Input/MouseInput/MouseInputFactoryPrivate.hpp"
#endif

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
				LogWrite(LogInfo, LogEngine, "[InputSystem::CreateInput]Creating Keyboard input");
				return new KeyboardInput();

			#if ETERNAL_PLATFORM_WINDOWS
			case InputType::INPUT_TYPE_XINPUT:
				LogWrite(LogInfo, LogEngine, "[InputSystem::CreateInput]Creating Xbox Pad input");
				return new XInput();
			#endif

			case InputType::INPUT_TYPE_SCE_PAD:
				#if ETERNAL_PLATFORM_WINDOWS
				LogWrite(LogInfo, LogEngine, "[InputSystem::CreateInput]Creating PS Pad input (Windows)");
				ETERNAL_BREAK();
				return nullptr;
				#endif
				#if ETERNAL_USE_PRIVATE
				return CreateScePadInputPrivate();
				#endif

			case InputType::INPUT_TYPE_MOUSE:
				#if ETERNAL_PLATFORM_WINDOWS
				LogWrite(LogInfo, LogEngine, "[InputSystem::CreateInput]Creating Mouse input");
				return new MouseInput();
				#endif
				#if ETERNAL_USE_PRIVATE
				return CreateMousePrivate();
				#endif

			case InputType::INPUT_TYPE_MULTI:	// Must be created with CreateMultiInput
			default:
				break;
			}
			ETERNAL_BREAK();
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
