#pragma once

namespace Eternal
{
	namespace InputSystem
	{
		enum class InputKey;
		enum class InputAxis;

		class Input
		{
		public:
			static constexpr float DefaultDeadZone = 0.2f;

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
			 * Key released
			 */
			virtual bool IsReleased(_In_ const InputKey& InKeyName);
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

			uint8_t* _States	= nullptr;
			float* _Axis		= nullptr;
		};
	}
}
