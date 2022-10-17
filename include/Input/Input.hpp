#pragma once

#define INPUT_CURRENT_STATE		(0x1)
#define INPUT_PREVIOUS_STATE	(0x2)

namespace Eternal
{
	namespace InputSystem
	{
		class Input
		{
		public:
			enum Key
			{
				A = 0,
				B,
				C,
				D,
				E,
				F,
				G,
				H,
				I,
				J,
				K,
				L,
				M,
				N,
				O,
				P,
				Q,
				R,
				S,
				T,
				U,
				V,
				W,
				X,
				Y,
				Z,
				UP,
				DOWN,
				LEFT,
				RIGHT,
				BACKSPACE,
				DEL,
				TAB,
				//CLEAR,
				RETURN,
				PAUSE,
				ESC,
				SPACE,
				KP0,
				KP1,
				KP2,
				KP3,
				KP4,
				KP5,
				KP6,
				KP7,
				KP8,
				KP9,
				KPPERIOD,
				KPDIV,
				KPMUL,
				KPMINUS,
				KPPLUS,
				KPENTER,
				KPEQ,
				INS,
				HOME,
				END,
				PGUP,
				PGDOWN,
				F1,
				F2,
				F3,
				F4,
				F5,
				F6,
				F7,
				F8,
				F9,
				F10,
				F11,
				F12,
				ALPHA0,
				ALPHA1,
				ALPHA2,
				ALPHA3,
				ALPHA4,
				ALPHA5,
				ALPHA6,
				ALPHA7,
				ALPHA8,
				ALPHA9,
				EXCLAIM,
				DOUBLEQUOTE,
				HASH,
				DOLLAR,
				AMPERSAND,
				QUOTE,
				LEFTPARENTHESIS,
				RIGHTPARENTHESIS,
				ASTERISK,
				PLUS,
				MINUS,
				PERIOD,
				SLASH,
				COLON,
				SEMICOLON,
				COMMA,
				LESS,
				EQ,
				GREATER,
				QUESTION,
				AT,
				LEFTBRACKET,
				RIGHTBRACKET,
				BACKSLASH,
				CARET,
				UNDERSCORE,
				BACKQUOTE,
				NUMLOCK,
				CAPSLOCK,
				SCROLLLOCK,
				LEFTCTRL,
				RIGHTCTRL,
				ALT,
				ALTGR,
				LEFTSHIFT,
				RIGHTSHIFT,
				LEFTWINDOWS,
				RIGHTWINDOWS,
				APPLICATION,
				PRINT,
				PRINTSCR,
				BREAK,

				MOUSE0,
				MOUSE1,
				MOUSE2,
				MOUSE3,
				MOUSE4,
				MOUSE5,
				MOUSE6,

				JOY0_UP,
				JOY0_DOWN,
				JOY0_LEFT,
				JOY0_RIGHT,

				JOY0_BUTTON0, // A
				JOY0_BUTTON1, // B
				JOY0_BUTTON2, // X
				JOY0_BUTTON3, // Y
				JOY0_BUTTON4, // LB
				JOY0_BUTTON5, // RB
				JOY0_BUTTON6, // Back
				JOY0_BUTTON7, // Start
				JOY0_BUTTON8, // LS
				JOY0_BUTTON9, // RS
				JOY0_BUTTON10,
				JOY0_BUTTON11,
				JOY0_BUTTON12,
				JOY0_BUTTON13,
				JOY0_BUTTON14,
				JOY0_BUTTON15,
				JOY0_BUTTON16,
				JOY0_BUTTON17,
				JOY0_BUTTON18,
				JOY0_BUTTON19,

				JOY1_UP,
				JOY1_DOWN,
				JOY1_LEFT,
				JOY1_RIGHT,

				JOY1_BUTTON0,
				JOY1_BUTTON1,
				JOY1_BUTTON2,
				JOY1_BUTTON3,
				JOY1_BUTTON4,
				JOY1_BUTTON5,
				JOY1_BUTTON6,
				JOY1_BUTTON7,
				JOY1_BUTTON8,
				JOY1_BUTTON9,
				JOY1_BUTTON10,
				JOY1_BUTTON11,
				JOY1_BUTTON12,
				JOY1_BUTTON13,
				JOY1_BUTTON14,
				JOY1_BUTTON15,
				JOY1_BUTTON16,
				JOY1_BUTTON17,
				JOY1_BUTTON18,
				JOY1_BUTTON19,

				JOY2_UP,
				JOY2_DOWN,
				JOY2_LEFT,
				JOY2_RIGHT,

				JOY2_BUTTON0,
				JOY2_BUTTON1,
				JOY2_BUTTON2,
				JOY2_BUTTON3,
				JOY2_BUTTON4,
				JOY2_BUTTON5,
				JOY2_BUTTON6,
				JOY2_BUTTON7,
				JOY2_BUTTON8,
				JOY2_BUTTON9,
				JOY2_BUTTON10,
				JOY2_BUTTON11,
				JOY2_BUTTON12,
				JOY2_BUTTON13,
				JOY2_BUTTON14,
				JOY2_BUTTON15,
				JOY2_BUTTON16,
				JOY2_BUTTON17,
				JOY2_BUTTON18,
				JOY2_BUTTON19,

				JOY3_UP,
				JOY3_DOWN,
				JOY3_LEFT,
				JOY3_RIGHT,

				JOY3_BUTTON0,
				JOY3_BUTTON1,
				JOY3_BUTTON2,
				JOY3_BUTTON3,
				JOY3_BUTTON4,
				JOY3_BUTTON5,
				JOY3_BUTTON6,
				JOY3_BUTTON7,
				JOY3_BUTTON8,
				JOY3_BUTTON9,
				JOY3_BUTTON10,
				JOY3_BUTTON11,
				JOY3_BUTTON12,
				JOY3_BUTTON13,
				JOY3_BUTTON14,
				JOY3_BUTTON15,
				JOY3_BUTTON16,
				JOY3_BUTTON17,
				JOY3_BUTTON18,
				JOY3_BUTTON19,

				KEY_COUNT,
				KEYBOARD_KEY_COUNT = MOUSE0,
				INVALID_KEY = ~0
			};

			enum Axis
			{
				MOUSE_X = 0,
				MOUSE_Y,

				JOY0_LX,
				JOY0_LY,
				JOY0_RX,
				JOY0_RY,
				JOY0_ZM,
				JOY0_ZP,

				JOY1_LX,
				JOY1_LY,
				JOY1_RX,
				JOY1_RY,
				JOY1_ZM,
				JOY1_ZP,

				JOY2_LX,
				JOY2_LY,
				JOY2_RX,
				JOY2_RY,
				JOY2_ZM,
				JOY2_ZP,

				JOY3_LX,
				JOY3_LY,
				JOY3_RX,
				JOY3_RY,
				JOY3_ZM,
				JOY3_ZP,

				AXIS_COUNT
			};

		public:
			Input();
			virtual ~Input();

			virtual void Update() = 0;
			virtual void NotifyKeyPressed(_In_ const Key& KeyName) = 0;
			virtual void NotifyKeyReleased(_In_ const Key& KeyName) = 0;
			virtual void NotifyAxis(_In_ const Axis& AxisName, _In_ float AxisValue) = 0;
			/**
			 * Key pressed
			 */
			virtual bool IsPressed(_In_ const Key& KeyName);
			/**
			 * Key released
			 */
			virtual bool IsReleased(_In_ const Key& KeyName);
			/**
			 * Key released frame
			 */
			virtual bool IsUp(_In_ const Key& KeyName);
			/**
			 * Key pressed frame
			 */
			virtual bool IsDown(_In_ const Key& KeyName);
			/**
			 * @return float between -1 and 1
			 */
			virtual float GetAxis(_In_ const Axis& AxisName);

		protected:
			uint8_t* _States	= nullptr;
			float* _Axis		= nullptr;
		};
	}
}
