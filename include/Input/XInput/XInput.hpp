#pragma once

#if ETERNAL_PLATFORM_WINDOWS || ETERNAL_PLATFORM_SCARLETT

#include "Input/Input.hpp"
#include <Windows.h>
#if ETERNAL_PLATFORM_WINDOWS
#include <Xinput.h>
#else
#include "Input/XInput/XInputLibraryPrivate.hpp"
#endif

namespace Eternal
{
	namespace InputSystem
	{
		class XInput : public Input
		{
		public:
			static constexpr DWORD InvalidChangedPacketNumber = ~0u;

			XInput();
			virtual void Update() override;
			virtual void NotifyKeyPressed(_In_ const InputKey& InKeyName) override { (void)InKeyName; }
			virtual void NotifyKeyReleased(_In_ const InputKey& InKeyName) override { (void)InKeyName; }
			virtual void NotifyAxis(_In_ const InputAxis& InAxisName, _In_ float InAxisValue) override
			{
				(void)InAxisName;
				(void)InAxisValue;
			}

		private:
			void _Pad();

			// Xbox360 Pad
			DWORD _Changed[XUSER_MAX_COUNT]	= {};
			DWORD _Users[XUSER_MAX_COUNT]	= {};
			uint32_t _UsersCount			= 0;
		};
	}
}

#endif
