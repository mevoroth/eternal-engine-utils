#pragma once

#include "Input/InputDefines.hpp"
#include <unordered_map>
#include <vector>
#include <string>

namespace Eternal
{
	namespace InputSystem
	{
		using namespace std;

		enum class InputKey;
		enum class InputAxis;
		class Input;

		struct InputKeyWeight
		{
			InputKeyWeight(_In_ const InputKey& InInputKey, float InKeyWeight = 1.0f)
				: KeyName(InInputKey)
				, KeyWeight(InKeyWeight)
			{
			}

			bool operator==(_In_ const InputKeyWeight& InOtherInputKeyWeight) const
			{
				return KeyName == InOtherInputKeyWeight.KeyName;
			}

			InputKey KeyName	= InputKey::KEY_INVALID;
			float KeyWeight		= 1.0f;
		};

		struct InputAxisWeight
		{
			InputAxisWeight(_In_ const InputAxis& InInputAxis, float InAxisWeight = 1.0f, float InAxisDeadZone = 0.0f)
				: AxisName(InInputAxis)
				, AxisWeight(InAxisWeight)
				, AxisDeadZone(InAxisDeadZone)
			{
			}

			bool operator==(_In_ const InputAxisWeight& InOtherInputAxisWeight) const
			{
				return AxisName == InOtherInputAxisWeight.AxisName;
			}

			InputAxis AxisName	= InputAxis::AXIS_COUNT;
			float AxisWeight	= 1.0f;
			float AxisDeadZone	= 0.0f;
		};

		struct InputActionKeyMapping
		{
			InputActionKeyMapping(_In_ const InputKeyWeight& InInputKeyWeight, _In_ const KeyEvent& InKeyEvent = KeyEvent::KEYEVENT_PRESSED)
				: KeyWeight(InInputKeyWeight)
				, MappingEvent(InKeyEvent)
			{
			}

			InputKeyWeight KeyWeight	= InputKeyWeight(InputKey::KEY_INVALID);
			KeyEvent MappingEvent		= KeyEvent::KEYEVENT_UP;
		};

		class InputMapping
		{
		public:

			InputMapping(_In_ Input& InInput);

			template<typename InputType>
			void MapAction(_In_ const string& InAction, _In_ const InputType& InInputMapping);
			void UnmapAction(_In_ const string& InAction);
			float GetActionFloat(_In_ const string& InAction) const;
			bool GetActionBool(_In_ const string& InAction) const;

		private:

			Input&											_Input;
			unordered_map<string, vector<InputKeyWeight>>	_ActionKeysMappingUp;
			unordered_map<string, vector<InputKeyWeight>>	_ActionKeysMappingDown;
			unordered_map<string, vector<InputKeyWeight>>	_ActionKeysMappingPressed;
			unordered_map<string, vector<InputAxisWeight>>	_ActionAxisMapping;
			unordered_map<string, vector<InputAxisWeight>>	_ActionAxisMappingWithDeadZone;

		};
	}
}
