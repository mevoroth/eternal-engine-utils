#pragma once

#include "Input/Input.hpp"

namespace Eternal
{
	namespace InputSystem
	{
		class AndroidTouchInput : public Input
		{
		public:

			AndroidTouchInput();

			virtual void Update() override;
			virtual void NotifyKeyPressed(_In_ const InputKey& InKeyName) override;
			virtual void NotifyKeyReleased(_In_ const InputKey& InKeyName) override;
			virtual void NotifyAxis(_In_ const InputAxis& InAxisName, _In_ float InAxisValue) override;

		};
	}
}
