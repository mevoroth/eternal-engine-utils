#pragma once

#include <string>

namespace Eternal
{
	namespace Components
	{
		class Animation;
	}

	namespace Import
	{
		using namespace std;
		using namespace Eternal::Components;

		class ImportAnim
		{
		public:

			Animation* Import(_In_ const string& InPath);

		};
	}
}
