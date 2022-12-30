#include "Input/Input.hpp"
#include "Math/Math.hpp"

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

		bool Input::IsDown(_In_ const Key& InKeyName)
		{
			return _States[InKeyName] == INPUT_PREVIOUS_STATE;
		}

		bool Input::IsUp(_In_ const Key& InKeyName)
		{
			return _States[InKeyName] == INPUT_CURRENT_STATE;
		}

		bool Input::IsPressed(_In_ const Key& InKeyName)
		{
			return (_States[InKeyName] & INPUT_CURRENT_STATE) != 0;
		}

		bool Input::IsReleased(_In_ const Key& InKeyName)
		{
			return (_States[InKeyName] & INPUT_CURRENT_STATE) != 0;
		}

		float Input::GetAxis(_In_ const Axis& InAxisName) const
		{
			return _Axis[InAxisName];
		}

		float Input::GetAxisWithDeadZone(_In_ const Axis& InAxisName, _In_ float InDeadZone) const
		{
			float AxisValue = GetAxis(InAxisName);
			float AbsAxisValue = Math::Abs(AxisValue);
			float AxisWithDeadZone = Math::Sign(AxisValue) * (AbsAxisValue < InDeadZone ? 0.0f : AbsAxisValue);
			return AxisWithDeadZone;
		}
	}
}
