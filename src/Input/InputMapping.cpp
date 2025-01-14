#include "Input/InputMapping.hpp"
#include "Input/Input.hpp"
#include <algorithm>
#include <functional>

namespace Eternal
{
	namespace InputSystem
	{
		using InputKeyEventFunctor	= bool (Input::*)(_In_ const InputKey& InKeyName);

		struct InputKeyEventFunctorMapping
		{
			InputKeyEventFunctorMapping(_In_ const InputKeyEventFunctor& InInputKeyEventFunction, _In_ const unordered_map<string, vector<InputKeyWeight>>& InMapping)
				: InputKeyEventFunction(InInputKeyEventFunction)
				, Mapping(InMapping)
			{
			}

			const InputKeyEventFunctor&								InputKeyEventFunction;
			const unordered_map<string, vector<InputKeyWeight>>&	Mapping;
		};

		template<typename InputType>
		static void InputMapping_MapAction(_In_ const string& InAction, _In_ const InputType& InInputMapping, _Inout_ unordered_map<string, vector<InputType>>& InOutActionMapping)
		{
			auto ActionKeysMappingIterator = InOutActionMapping.find(InAction);
			if (ActionKeysMappingIterator == InOutActionMapping.end())
				ActionKeysMappingIterator = InOutActionMapping.insert({ InAction, {} }).first;

			auto FoundMapping = find(ActionKeysMappingIterator->second.begin(), ActionKeysMappingIterator->second.end(), InInputMapping);
			if (FoundMapping == ActionKeysMappingIterator->second.end())
				ActionKeysMappingIterator->second.emplace_back(InInputMapping);
			else
				*FoundMapping = InInputMapping;
		}

		InputMapping::InputMapping(_In_ Input& InInput)
			: _Input(InInput)
		{
		}

		template<> void InputMapping::MapAction(_In_ const string& InAction, _In_ const InputActionKeyMapping& InInputMapping)
		{
			switch (InInputMapping.MappingEvent)
			{
			case KeyEvent::KEYEVENT_UP:
				InputMapping_MapAction(InAction, InInputMapping.KeyWeight, _ActionKeysMappingUp);
				break;
			case KeyEvent::KEYEVENT_DOWN:
				InputMapping_MapAction(InAction, InInputMapping.KeyWeight, _ActionKeysMappingDown);
				break;
			case KeyEvent::KEYEVENT_PRESSED:
				InputMapping_MapAction(InAction, InInputMapping.KeyWeight, _ActionKeysMappingPressed);
				break;
			default:
				break;
			}
		}

		template<> void InputMapping::MapAction(_In_ const string& InAction, _In_ const InputAxisWeight& InInputMapping)
		{
			InputMapping_MapAction(InAction, InInputMapping, InInputMapping.AxisDeadZone != 0.0f ? _ActionAxisMappingWithDeadZone : _ActionAxisMapping);
		}

		void InputMapping::UnmapAction(_In_ const string& InAction)
		{
			_ActionKeysMappingUp.erase(InAction);
			_ActionKeysMappingDown.erase(InAction);
			_ActionKeysMappingPressed.erase(InAction);
			_ActionAxisMapping.erase(InAction);
			_ActionAxisMappingWithDeadZone.erase(InAction);
		}

		float InputMapping::GetActionFloat(_In_ const string& InAction) const
		{
			auto GetActionKeyPerformed = [this, &InAction](_In_ const unordered_map<string, vector<InputKeyWeight>>& InActionKeysMapping, _In_ const InputKeyEventFunctor& InInputKeyEventFunction) -> float
			{
				auto ActionKeyMappingIterator = InActionKeysMapping.find(InAction);
				if (ActionKeyMappingIterator != InActionKeysMapping.cend())
				{
					const vector<InputKeyWeight>& InputKeyWeights = ActionKeyMappingIterator->second;

					for (uint32_t KeyIndex = 0; KeyIndex < InputKeyWeights.size(); ++KeyIndex)
					{
						if ((_Input.*InInputKeyEventFunction)(InputKeyWeights[KeyIndex].KeyName))
							return InputKeyWeights[KeyIndex].KeyWeight;
					}
				}

				return 0.0f;
			};

			auto GetActionAxisPerformed = [this, &InAction](_In_ const unordered_map<string, vector<InputAxisWeight>>& InActionAxisMapping, _In_ const function<float(_In_ const InputAxisWeight& InInputAxisWeight)>& InInputAxisEventFunction) -> float
			{
				auto ActionAxisMappingIterator = InActionAxisMapping.find(InAction);
				if (ActionAxisMappingIterator != InActionAxisMapping.cend())
				{
					const vector<InputAxisWeight>& InputAxisWeights = ActionAxisMappingIterator->second;

					for (uint32_t AxisIndex = 0; AxisIndex < InputAxisWeights.size(); ++AxisIndex)
					{
						float AxisValue = InInputAxisEventFunction(InputAxisWeights[AxisIndex]);
						if (AxisValue != 0.0f)
							return AxisValue * InputAxisWeights[AxisIndex].AxisWeight;
					}
				}
			};

			auto GetAxis = [this](_In_ const InputAxisWeight& InInputAxisWeight) -> float
			{
				return _Input.GetAxis(InInputAxisWeight.AxisName);
			};

			auto GetAxisWithDeadZone = [this](_In_ const InputAxisWeight& InInputAxisWeight) -> float
			{
				return _Input.GetAxisWithDeadZone(InInputAxisWeight.AxisName, InInputAxisWeight.AxisDeadZone);
			};

			InputKeyEventFunctorMapping InputKeyEventFunctorMappings[] =
			{
				{ &Input::IsPressed,	_ActionKeysMappingPressed },
				{ &Input::IsDown,		_ActionKeysMappingDown },
				{ &Input::IsUp,			_ActionKeysMappingUp }
			};

			for (uint32_t MappingIndex = 0; MappingIndex < ETERNAL_ARRAYSIZE(InputKeyEventFunctorMappings); ++MappingIndex)
			{
				float ActionPerformed = GetActionKeyPerformed(InputKeyEventFunctorMappings[MappingIndex].Mapping, InputKeyEventFunctorMappings[MappingIndex].InputKeyEventFunction);
				if (ActionPerformed != 0.0f)
					return ActionPerformed;
			}

			{
				float ActionPerformed = GetActionAxisPerformed(_ActionAxisMapping, GetAxis);
				if (ActionPerformed != 0.0f)
					return ActionPerformed;
			}

			{
				float ActionPerformed = GetActionAxisPerformed(_ActionAxisMappingWithDeadZone, GetAxisWithDeadZone);
				if (ActionPerformed != 0.0f)
					return ActionPerformed;
			}

			return 0.0f;
		}

		bool InputMapping::GetActionBool(_In_ const string& InAction) const
		{
			return GetActionFloat(InAction) != 0.0f;
		}
	}
}
