#include "Input/MouseInput/MouseInput.hpp"

namespace Eternal
{
	namespace InputSystem
	{
		MouseInput::MouseInput()
			: Input()
		{
		}

		void MouseInput::Update()
		{
			for (uint32_t Key = MOUSE0; Key <= MOUSE6; ++Key)
			{
				_States[Key] = (_States[Key] << 1) & (INPUT_CURRENT_STATE | INPUT_PREVIOUS_STATE);
			}
			for (uint32_t RecordIndex = 0; RecordIndex < _KeyRecords.size(); ++RecordIndex)
			{
				KeyRecord& Record = _KeyRecords[RecordIndex];
				if (Record.State == DOWN)
					_States[Record.KeyName] |= 0x1;
				else
					_States[Record.KeyName] &= ~(0x1);
			}
			_KeyRecords.clear();
		}

		void MouseInput::NotifyKeyPressed(_In_ const Key& InKeyName)
		{
			if (InKeyName < MOUSE0 || InKeyName > MOUSE6)
				return;

			_KeyRecords.push_back(KeyRecord(InKeyName, DOWN));
		}

		void MouseInput::NotifyKeyReleased(_In_ const Key& InKeyName)
		{
			if (InKeyName < MOUSE0 || InKeyName > MOUSE6)
				return;

			_KeyRecords.push_back(KeyRecord(InKeyName, UP));
		}

		void MouseInput::NotifyAxis(_In_ const Axis& InAxisName, _In_ float InAxisValue)
		{
			_Axis[InAxisName] = InAxisValue;
		}
	}
}
