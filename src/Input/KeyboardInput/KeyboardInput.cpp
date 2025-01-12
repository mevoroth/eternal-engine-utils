#include "Input/KeyboardInput/KeyboardInput.hpp"
#include "Types/Enums.hpp"

namespace Eternal
{
	namespace InputSystem
	{
		using namespace Types;

		KeyboardInput::KeyboardInput()
			: Input()
		{
		}

		void KeyboardInput::Update()
		{
			for (uint32_t Key = ToUInt(InputKey::KEY_A); Key < ToUInt(InputKey::KEY_COUNT_KEYBOARD); ++Key)
			{
				_States[Key] = (_States[Key] << 1) & (INPUT_CURRENT_STATE | INPUT_PREVIOUS_STATE);
			}
			for (uint32_t RecordIndex = 0; RecordIndex < _KeyRecords.size(); ++RecordIndex)
			{
				KeyRecord& Record = _KeyRecords[RecordIndex];
				if (Record.State == KeyState::KEYSTATE_DOWN)
					_States[ToUInt(Record.KeyName)] |= 0x1;
				else
					_States[ToUInt(Record.KeyName)] &= ~(0x1);
			}
			// Probably need mutex
			_KeyRecords.clear();
		}

		void KeyboardInput::NotifyKeyPressed(_In_ const InputKey& InKeyName)
		{
			if (ToUInt(InKeyName) >= ToUInt(InputKey::KEY_MOUSE0) && ToUInt(InKeyName) <= ToUInt(InputKey::KEY_MOUSE6))
				return;

			// Probably need mutex
			_KeyRecords.push_back(KeyRecord(InKeyName, KeyState::KEYSTATE_DOWN));
		}

		void KeyboardInput::NotifyKeyReleased(_In_ const InputKey& InKeyName)
		{
			if (ToUInt(InKeyName) >= ToUInt(InputKey::KEY_MOUSE0) && ToUInt(InKeyName) <= ToUInt(InputKey::KEY_MOUSE6))
				return;

			// Probably need mutex
			_KeyRecords.push_back(KeyRecord(InKeyName, KeyState::KEYSTATE_UP));
		}

		void KeyboardInput::NotifyAxis(_In_ const InputAxis& InAxisName, _In_ float InAxisValue)
		{
			(void)InAxisName;
			(void)InAxisValue;
		}
	}
}
