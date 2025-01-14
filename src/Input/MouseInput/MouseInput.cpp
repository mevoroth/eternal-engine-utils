#include "Input/MouseInput/MouseInput.hpp"
#include "Input/InputDefines.hpp"
#include "Types/Enums.hpp"

namespace Eternal
{
	namespace InputSystem
	{
		using namespace Eternal::Types;

		MouseInput::MouseInput()
			: Input()
		{
		}

		void MouseInput::Update()
		{
			for (uint32_t Key = ToUInt(InputKey::KEY_MOUSE0); Key <= ToUInt(InputKey::KEY_MOUSE6); ++Key)
				_States[Key] = (_States[Key] << 1) & (InputCurrentState | InputPreviousState);

			for (uint32_t RecordIndex = 0; RecordIndex < _KeyRecords.size(); ++RecordIndex)
			{
				KeyRecord& Record = _KeyRecords[RecordIndex];
				if (Record.State == KeyState::KEYSTATE_DOWN)
					_States[ToUInt(Record.KeyName)] |= 0x1;
				else
					_States[ToUInt(Record.KeyName)] &= ~(0x1);
			}
			_KeyRecords.clear();
		}

		void MouseInput::NotifyKeyPressed(_In_ const InputKey& InKeyName)
		{
			if (ToUInt(InKeyName) < ToUInt(InputKey::KEY_MOUSE0) || ToUInt(InKeyName) > ToUInt(InputKey::KEY_MOUSE6))
				return;

			_KeyRecords.push_back(KeyRecord(InKeyName, KeyState::KEYSTATE_DOWN));
		}

		void MouseInput::NotifyKeyReleased(_In_ const InputKey& InKeyName)
		{
			if (ToUInt(InKeyName) < ToUInt(InputKey::KEY_MOUSE0) || ToUInt(InKeyName) > ToUInt(InputKey::KEY_MOUSE6))
				return;

			_KeyRecords.push_back(KeyRecord(InKeyName, KeyState::KEYSTATE_UP));
		}

		void MouseInput::NotifyAxis(_In_ const InputAxis& InAxisName, _In_ float InAxisValue)
		{
			_Axis[ToUInt(InAxisName)] = InAxisValue;
		}
	}
}
