#pragma once

#include "Input/Input.hpp"
#include "Input/InputDefines.hpp"

namespace Eternal
{
	namespace InputSystem
	{
		class MouseInput : public Input
		{
		public:

			MouseInput();

			virtual void Update() override;
			virtual void NotifyKeyPressed(_In_ const InputKey& InKeyName) override;
			virtual void NotifyKeyReleased(_In_ const InputKey& InKeyName) override;
			virtual void NotifyAxis(_In_ const InputAxis& InAxisName, _In_ float InAxisValue) override;

		private:

			std::vector<KeyRecord> _KeyRecords;

		};
	}
}
