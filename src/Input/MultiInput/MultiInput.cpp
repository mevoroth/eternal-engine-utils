#include "Input/MultiInput/MultiInput.hpp"

#include <cmath>

namespace Eternal
{
	namespace InputSystem
	{
		MultiInput::MultiInput()
			: Input()
		{
			Initialize(this);
		}

		void MultiInput::Update()
		{
			for (uint32_t InputIndex = 0; InputIndex < _Inputs.size(); ++InputIndex)
				_Inputs[InputIndex]->Update();
		}

		void MultiInput::NotifyKeyPressed(_In_ const Key& InKeyName)
		{
			for (uint32_t InputIndex = 0; InputIndex < _Inputs.size(); ++InputIndex)
				_Inputs[InputIndex]->NotifyKeyPressed(InKeyName);
		}
		void MultiInput::NotifyKeyReleased(_In_ const Key& InKeyName)
		{
			for (uint32_t InputIndex = 0; InputIndex < _Inputs.size(); ++InputIndex)
				_Inputs[InputIndex]->NotifyKeyReleased(InKeyName);
		}
		void MultiInput::NotifyAxis(_In_ const Axis& InAxisName, _In_ float InAxisValue)
		{
			for (uint32_t InputIndex = 0; InputIndex < _Inputs.size(); ++InputIndex)
				_Inputs[InputIndex]->NotifyAxis(InAxisName, InAxisValue);
		}
		bool MultiInput::IsPressed(_In_ const Key& InKeyName)
		{
			bool Pressed = false;
			for (uint32_t InputIndex = 0; !Pressed && InputIndex < _Inputs.size(); ++InputIndex)
				Pressed = _Inputs[InputIndex]->IsPressed(InKeyName);
			return Pressed;
		}
		bool MultiInput::IsReleased(_In_ const Key& InKeyName)
		{
			bool Released = false;
			for (uint32_t InputIndex = 0; !Released && InputIndex < _Inputs.size(); ++InputIndex)
				Released = _Inputs[InputIndex]->IsReleased(InKeyName);
			return Released;
		}
		bool MultiInput::IsUp(_In_ const Key& InKeyName)
		{
			bool Up = false;
			for (uint32_t InputIndex = 0; !Up && InputIndex < _Inputs.size(); ++InputIndex)
				Up = _Inputs[InputIndex]->IsUp(InKeyName);
			return Up;
		}
		bool MultiInput::IsDown(_In_ const Key& InKeyName)
		{
			bool Down = false;
			for (uint32_t InputIndex = 0; !Down && InputIndex < _Inputs.size(); ++InputIndex)
				Down = _Inputs[InputIndex]->IsDown(InKeyName);
			return Down;
		}
		float MultiInput::GetAxis(_In_ const Axis& InAxisName)
		{
			float AxisValue = 0.f;
			for (uint32_t InputIndex = 0; InputIndex < _Inputs.size(); ++InputIndex)
			{
				float CurrentAxisValue = _Inputs[InputIndex]->GetAxis(InAxisName);
				if (CurrentAxisValue != 0.f)
				{
					ETERNAL_ASSERT(AxisValue == 0.f);
					AxisValue = CurrentAxisValue;
				}
			}
			return AxisValue;
		}

		void MultiInput::Add(_In_ Input* InInput)
		{
			ETERNAL_ASSERT(InInput);
			_Inputs.push_back(InInput);
		}
	}
}
