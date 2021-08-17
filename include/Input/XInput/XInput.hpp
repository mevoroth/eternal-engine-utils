#ifndef _X_INPUT_HPP_
#define _X_INPUT_HPP_

#include "Input/Input.hpp"
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>
#include <Xinput.h>

namespace Eternal
{
	namespace InputSystem
	{
		class XInput : public Input
		{
		public:
			XInput();
			virtual void Update() override;
			virtual void NotifyKeyPressed(_In_ const Key& KeyName) override {}
			virtual void NotifyKeyReleased(_In_ const Key& KeyName) override {}
			virtual void NotifyAxis(_In_ const Axis& AxisName, _In_ float AxisValue) override {};

		private:
			void _Pad();

			// Xbox360 Pad
			DWORD _Changed[XUSER_MAX_COUNT];
			DWORD _Users[XUSER_MAX_COUNT];
			uint32_t _UsersCount = 0;
		};
	}
}

#endif
