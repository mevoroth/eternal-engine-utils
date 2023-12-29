#pragma once

#include "Input/Input.hpp"

namespace Eternal
{
	namespace InputSystem
	{
		class MouseInput : public Input
		{
		public:
			enum KeyState
			{
				DOWN = 0,
				UP = 1
			};

			MouseInput();

			virtual void Update() override;
			virtual void NotifyKeyPressed(_In_ const Key& InKeyName) override;
			virtual void NotifyKeyReleased(_In_ const Key& InKeyName) override;
			virtual void NotifyAxis(_In_ const Axis& InAxisName, _In_ float InAxisValue) override;

		private:
			struct KeyRecord
			{
				Key KeyName;
				KeyState State;
				KeyRecord(const Key& InKeyName, const KeyState& InState)
					: KeyName(InKeyName)
					, State(InState)
				{
				}
			};

			std::vector<KeyRecord> _KeyRecords;

		};
	}
}
