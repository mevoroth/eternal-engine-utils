#pragma once

#include <array>
#include "Input/InputDefines.hpp"
#include "Types/Enums.hpp"

namespace Eternal
{
	namespace InputSystem
	{
		using namespace std;
		using namespace Eternal::Types;

		enum class InputKey;
		enum class InputAxis;

		class Input
		{
		public:

			Input();
			virtual ~Input();

			virtual void Update() = 0;
			virtual void NotifyKeyPressed(_In_ const InputKey& InKeyName) = 0;
			virtual void NotifyKeyReleased(_In_ const InputKey& InKeyName) = 0;
			virtual void NotifyAxis(_In_ const InputAxis& InAxisName, _In_ float InAxisValue) = 0;
			/**
			 * Key pressed
			 */
			virtual bool IsPressed(_In_ const InputKey& InKeyName);
			/**
			 * Key released frame
			 */
			virtual bool IsUp(_In_ const InputKey& InKeyName);
			/**
			 * Key pressed frame
			 */
			virtual bool IsDown(_In_ const InputKey& InKeyName);
			/**
			 * @return float between -1 and 1
			 */
			virtual float GetAxis(_In_ const InputAxis& InAxisName) const;
			/**
			 * @return float between -1 and 1
			 */
			float GetAxisWithDeadZone(_In_ const InputAxis& InAxisName, _In_ float InDeadZone = DefaultDeadZone) const;

		protected:

			array<uint8_t, ToUInt(InputKey::KEY_COUNT)>	_States;
			array<float, ToUInt(InputAxis::AXIS_COUNT)>	_Axis;
		};
	}
}
