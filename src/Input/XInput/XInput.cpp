#if ETERNAL_PLATFORM_WINDOWS || ETERNAL_PLATFORM_SCARLETT

#include "Input/XInput/XInput.hpp"

#include <string>

namespace Eternal
{
	namespace InputSystem
	{
		XInput::XInput()
			: Input()
		{
			for (DWORD User = 0; User < XUSER_MAX_COUNT; ++User)
			{
				_Changed[User] = ~0u;
			}

			for (DWORD User = 0; User < XUSER_MAX_COUNT; ++User)
			{
				XINPUT_CAPABILITIES XInputCapabilities;
				DWORD Result = XInputGetCapabilities(
					User,
					0,
					&XInputCapabilities
				);

				if (Result == ERROR_SUCCESS)
				{
					_Users[_UsersCount++];
				}
			}
		}

		void XInput::Update()
		{
			_Pad();
		}

		void XInput::_Pad()
		{
			// Update Pad
			for (uint32_t UserIndex = 0; UserIndex < _UsersCount; ++UserIndex)
			{
				XINPUT_STATE State;
				ZeroMemory(&State, sizeof(XINPUT_STATE));

				DWORD User = _Users[UserIndex];

				if (XInputGetState(User, &State) == ERROR_SUCCESS)
				{
					for (int StateIndex = JOY0_UP + User * 24, StateCount = User + 24; StateIndex < StateCount; ++StateIndex)
					{
						_States[StateIndex] = (_States[StateIndex] << 1) & (INPUT_CURRENT_STATE | INPUT_PREVIOUS_STATE);
					}
					if (State.dwPacketNumber != _Changed[User])
					{
						int Pad = User * 6;
						_Axis[JOY0_LX + Pad] = (float)State.Gamepad.sThumbLX * (1.0f / 32767.f);
						_Axis[JOY0_LY + Pad] = (float)State.Gamepad.sThumbLY * (1.0f / 32767.f);
						_Axis[JOY0_RX + Pad] = (float)State.Gamepad.sThumbRX * (1.0f / 32767.f);
						_Axis[JOY0_RY + Pad] = (float)State.Gamepad.sThumbRY * (1.0f / 32767.f);
						_Axis[JOY0_ZM + Pad] = (float)State.Gamepad.bLeftTrigger * (1.0f / 255.0f);
						_Axis[JOY0_ZP + Pad] = (float)State.Gamepad.bRightTrigger * (1.0f / 255.0f);

						Pad = User * 24; // 20 buttons + D-Pad (4 buttons)
						_States[JOY0_UP + Pad] |= (State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) ? 0x1 : 0x0;
						_States[JOY0_DOWN + Pad] |= (State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) ? 0x1 : 0x0;
						_States[JOY0_LEFT + Pad] |= (State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) ? 0x1 : 0x0;
						_States[JOY0_RIGHT + Pad] |= (State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) ? 0x1 : 0x0;
						_States[JOY0_BUTTON0 + Pad] |= (State.Gamepad.wButtons & XINPUT_GAMEPAD_A) ? 0x1 : 0x0;
						_States[JOY0_BUTTON1 + Pad] |= (State.Gamepad.wButtons & XINPUT_GAMEPAD_B) ? 0x1 : 0x0;
						_States[JOY0_BUTTON2 + Pad] |= (State.Gamepad.wButtons & XINPUT_GAMEPAD_X) ? 0x1 : 0x0;
						_States[JOY0_BUTTON3 + Pad] |= (State.Gamepad.wButtons & XINPUT_GAMEPAD_Y) ? 0x1 : 0x0;
						_States[JOY0_BUTTON4 + Pad] |= (State.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) ? 0x1 : 0x0;
						_States[JOY0_BUTTON5 + Pad] |= (State.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) ? 0x1 : 0x0;
						_States[JOY0_BUTTON6 + Pad] |= (State.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) ? 0x1 : 0x0;
						_States[JOY0_BUTTON7 + Pad] |= (State.Gamepad.wButtons & XINPUT_GAMEPAD_START) ? 0x1 : 0x0;
						_States[JOY0_BUTTON8 + Pad] |= (State.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) ? 0x1 : 0x0;
						_States[JOY0_BUTTON9 + Pad] |= (State.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) ? 0x1 : 0x0;

						_Changed[User] = State.dwPacketNumber;
					}
					else
					{
						for (int StateIndex = JOY0_UP + User * 24, StateCount = User + 24; StateIndex < StateCount; ++StateIndex)
						{
							ETERNAL_ASSERT(!(_States[User] & 0x1));
							_States[StateIndex] |= (_States[StateIndex] & 0x2) >> 1; // _States[User] |= (_States[User] & (0x1 << 1)) >> 1;
						}
					}
				}
				else
				{
					if (_Changed[User] != -1)
					{
						for (int StateIndex = JOY0_UP + User * 24, StateCount = User + 24; StateIndex < StateCount; ++StateIndex)
						{
							_States[StateIndex] = 0;
						}
						_Changed[User] = ~0u;
					}
				}
			}
		}
	}
}

#endif
