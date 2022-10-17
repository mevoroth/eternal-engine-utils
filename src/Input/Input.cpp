#include "Input/Input.hpp"

namespace Eternal
{
	namespace InputSystem
	{
		Input::Input()
		{
			//ETERNAL_ASSERT(!_Inst);

			_States = new uint8_t[KEY_COUNT];
			for (int i = 0; i < KEY_COUNT; ++i)
			{
				_States[i] = 0;
			}

			_Axis = new float[AXIS_COUNT];
			for (int i = 0; i < AXIS_COUNT; ++i)
			{
				_Axis[i] = 0.f;
			}

			//_Inst = this;
		}

		Input::~Input()
		{
			delete[] _States;
			_States = nullptr;
			delete[] _Axis;
			_Axis = nullptr;
		}

		bool Input::IsDown(_In_ const Key& KeyName)
		{
			return _States[KeyName] == INPUT_PREVIOUS_STATE;
		}

		bool Input::IsUp(_In_ const Key& KeyName)
		{
			return _States[KeyName] == INPUT_CURRENT_STATE;
		}

		bool Input::IsPressed(_In_ const Key& KeyName)
		{
			return (_States[KeyName] & INPUT_CURRENT_STATE) != 0;
		}

		bool Input::IsReleased(_In_ const Key& KeyName)
		{
			return (_States[KeyName] & INPUT_CURRENT_STATE) != 0;
		}

		float Input::GetAxis(_In_ const Axis& AxisName)
		{
			return _Axis[AxisName];
		}
	}
}
