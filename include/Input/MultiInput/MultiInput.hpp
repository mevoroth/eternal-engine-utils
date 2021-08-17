#pragma once

#include "Input/Input.hpp"

namespace Eternal
{
	namespace InputSystem
	{
		using namespace std;

		class MultiInput : public Input
		{
		public:
			MultiInput();

			virtual void Update() override;
			virtual void NotifyKeyPressed(_In_ const Key& InKeyName) override;
			virtual void NotifyKeyReleased(_In_ const Key& InKeyName) override;
			virtual void NotifyAxis(_In_ const Axis& InAxisName, _In_ float InAxisValue) override;
			virtual bool IsPressed(_In_ const Key& InKeyName) override;
			virtual bool IsReleased(_In_ const Key& InKeyName) override;
			virtual bool IsUp(_In_ const Key& InKeyName) override;
			virtual bool IsDown(_In_ const Key& InKeyName) override;
			virtual float GetAxis(_In_ const Axis& InAxisName) override;

			void Add(_In_ Input* InInput);

		private:
			vector<Input*> _Inputs;
		};
	}
}
