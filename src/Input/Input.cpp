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
			_States.fill(0);
			_Axis.fill(0.0f);
		}

		Input::~Input()
		{
		}

		bool Input::IsDown(_In_ const InputKey& InKeyName)
		{
			return _States[ToUInt(InKeyName)] == InputPreviousState;
		}

		bool Input::IsUp(_In_ const InputKey& InKeyName)
		{
			return _States[ToUInt(InKeyName)] == InputCurrentState;
		}

		bool Input::IsPressed(_In_ const InputKey& InKeyName)
		{
			return (_States[ToUInt(InKeyName)] & InputCurrentState) != 0;
		}

		bool Input::IsReleased(_In_ const InputKey& InKeyName)
		{
			return (_States[ToUInt(InKeyName)] & InputCurrentState) != 0;
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
