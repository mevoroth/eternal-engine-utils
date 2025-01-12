#include "Input/Input.hpp"
#include "Input/InputDefines.hpp"
#include "Math/Math.hpp"
#include "Types/Enums.hpp"

namespace Eternal
{
	namespace InputSystem
	{
		using namespace Eternal::Types;

		Input::Input()
		{
			_States = new uint8_t[ToUInt(InputKey::KEY_COUNT)];
			for (uint32_t Key = 0; Key < ToUInt(InputKey::KEY_COUNT); ++Key)
			{
				_States[Key] = 0;
			}

			_Axis = new float[ToUInt(InputAxis::AXIS_COUNT)];
			for (uint32_t Axis = 0; Axis < ToUInt(InputAxis::AXIS_COUNT); ++Axis)
			{
				_Axis[Axis] = 0.0f;
			}
		}

		Input::~Input()
		{
			delete[] _States;
			_States = nullptr;
			delete[] _Axis;
			_Axis = nullptr;
		}

		bool Input::IsDown(_In_ const InputKey& InKeyName)
		{
			return _States[ToUInt(InKeyName)] == INPUT_PREVIOUS_STATE;
		}

		bool Input::IsUp(_In_ const InputKey& InKeyName)
		{
			return _States[ToUInt(InKeyName)] == INPUT_CURRENT_STATE;
		}

		bool Input::IsPressed(_In_ const InputKey& InKeyName)
		{
			return (_States[ToUInt(InKeyName)] & INPUT_CURRENT_STATE) != 0;
		}

		bool Input::IsReleased(_In_ const InputKey& InKeyName)
		{
			return (_States[ToUInt(InKeyName)] & INPUT_CURRENT_STATE) != 0;
		}

		float Input::GetAxis(_In_ const InputAxis& InAxisName) const
		{
			return _Axis[ToUInt(InAxisName)];
		}

		float Input::GetAxisWithDeadZone(_In_ const InputAxis& InAxisName, _In_ float InDeadZone) const
		{
			float AxisValue = GetAxis(InAxisName);
			float AbsAxisValue = Math::Abs(AxisValue);
			float AxisWithDeadZone = Math::Sign(AxisValue) * (AbsAxisValue < InDeadZone ? 0.0f : AbsAxisValue);
			return AxisWithDeadZone;
		}
	}
}
