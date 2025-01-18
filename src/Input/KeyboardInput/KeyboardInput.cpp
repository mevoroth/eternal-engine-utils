#include "Input/KeyboardInput/KeyboardInput.hpp"
#include "Types/Enums.hpp"
#include <algorithm>

namespace Eternal
{
	namespace InputSystem
	{
		using namespace Types;

		KeyboardInput::KeyboardInput()
			: Input()
		{
			_KeyDowns.reserve(16);
			_KeyUps.reserve(16);
		}

		void KeyboardInput::Update()
		{
			for (uint32_t Key = ToUInt(InputKey::KEY_A); Key < ToUInt(InputKey::KEY_COUNT_KEYBOARD); ++Key)
				_States[Key] = (_States[Key] << 1) & (InputCurrentState | InputPreviousState);

			for (uint32_t Key = 0; Key < _KeyUps.size(); ++Key)
			{
				auto FoundKey = std::find(_KeyDowns.begin(), _KeyDowns.end(), _KeyUps[Key]);
				if (FoundKey != _KeyDowns.end())
				{
					std::swap(*FoundKey, _KeyDowns.back());
					_KeyDowns.pop_back();
				}
			}
			_KeyUps.clear();

			for (uint32_t Key = 0; Key < _KeyDowns.size(); ++Key)
				_States[ToUInt(_KeyDowns[Key])] |= ToUInt(KeyState::KEYSTATE_DOWN);
		}

		void KeyboardInput::NotifyKeyPressed(_In_ const InputKey& InKeyName)
		{
			if (ToUInt(InKeyName) >= ToUInt(InputKey::KEY_MOUSE0) && ToUInt(InKeyName) <= ToUInt(InputKey::KEY_MOUSE6))
				return;

			_KeyDowns.push_back(InKeyName);
		}

		void KeyboardInput::NotifyKeyReleased(_In_ const InputKey& InKeyName)
		{
			if (ToUInt(InKeyName) >= ToUInt(InputKey::KEY_MOUSE0) && ToUInt(InKeyName) <= ToUInt(InputKey::KEY_MOUSE6))
				return;

			_KeyUps.push_back(InKeyName);
		}

		void KeyboardInput::NotifyAxis(_In_ const InputAxis& InAxisName, _In_ float InAxisValue)
		{
			(void)InAxisName;
			(void)InAxisValue;
		}
	}
}
