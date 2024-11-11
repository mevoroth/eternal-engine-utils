#pragma once

#if ETERNAL_PLATFORM_WINDOWS || ETERNAL_PLATFORM_SCARLETT

#include "Input/Input.hpp"
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
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
			XInput();
			virtual void Update() override;
			virtual void NotifyKeyPressed(_In_ const Key& InKeyName) override { (void)InKeyName; }
			virtual void NotifyKeyReleased(_In_ const Key& InKeyName) override { (void)InKeyName; }
			virtual void NotifyAxis(_In_ const Axis& InAxisName, _In_ float InAxisValue) override
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
