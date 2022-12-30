#include "Input/WinInput/WinInput.hpp"

namespace Eternal
{
	namespace InputSystem
	{
		WinInput::WinInput()
			: Input()
		{
		}

		void WinInput::Update()
		{
			for (uint32_t Key = A; Key < KEYBOARD_KEY_COUNT; ++Key)
			{
				_States[Key] = (_States[Key] << 1) & (INPUT_CURRENT_STATE | INPUT_PREVIOUS_STATE);
			}
			for (uint32_t RecordIndex = 0; RecordIndex < _KeyRecords.size(); ++RecordIndex)
			{
				KeyRecord& Record = _KeyRecords[RecordIndex];
				if (Record.State == WinInput::DOWN)
					_States[Record.KeyName] |= 0x1;
				else
					_States[Record.KeyName] &= ~(0x1);
			}
			// Probably need mutex
			_KeyRecords.clear();
		}

		void WinInput::NotifyKeyPressed(_In_ const Key& InKeyName)
		{
			// Probably need mutex
			_KeyRecords.push_back(KeyRecord(InKeyName, DOWN));
		}

		void WinInput::NotifyKeyReleased(_In_ const Key& InKeyName)
		{
			// Probably need mutex
			_KeyRecords.push_back(KeyRecord(InKeyName, UP));
		}

		void WinInput::NotifyAxis(_In_ const Axis& InAxisName, _In_ float InAxisValue)
		{
			_Axis[InAxisName] = InAxisValue;
		}
	}
}
