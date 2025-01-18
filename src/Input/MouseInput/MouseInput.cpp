#include "Input/MouseInput/MouseInput.hpp"
#include "Input/InputDefines.hpp"
#include "Types/Enums.hpp"
#include <algorithm>

namespace Eternal
{
	namespace InputSystem
	{
		using namespace Eternal::Types;

		MouseInput::MouseInput()
			: Input()
		{
			_KeyDowns.reserve(16);
			_KeyUps.reserve(16);
		}

		void MouseInput::Update()
		{
			for (uint32_t Key = ToUInt(InputKey::KEY_MOUSE0); Key <= ToUInt(InputKey::KEY_MOUSE6); ++Key)
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

		void MouseInput::NotifyKeyPressed(_In_ const InputKey& InKeyName)
		{
			if (ToUInt(InKeyName) < ToUInt(InputKey::KEY_MOUSE0) || ToUInt(InKeyName) > ToUInt(InputKey::KEY_MOUSE6))
				return;

			_KeyDowns.push_back(InKeyName);
		}

		void MouseInput::NotifyKeyReleased(_In_ const InputKey& InKeyName)
		{
			if (ToUInt(InKeyName) < ToUInt(InputKey::KEY_MOUSE0) || ToUInt(InKeyName) > ToUInt(InputKey::KEY_MOUSE6))
				return;

			_KeyUps.push_back(InKeyName);
		}

		void MouseInput::NotifyAxis(_In_ const InputAxis& InAxisName, _In_ float InAxisValue)
		{
			_Axis[ToUInt(InAxisName)] = InAxisValue;
		}
	}
}
