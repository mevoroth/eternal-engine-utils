#pragma once

#include <string>

namespace Eternal
{
	namespace Components
	{
		class Material;
	}

	namespace Resources
	{
		struct MaterialDependency;
	}

	namespace Import
	{
		class ImportMat
		{
		public:

			ImportMat();

			Components::Material* Import(_In_ const std::string& InPath, _Inout_ Resources::MaterialDependency& InOutMaterialDependency);

		};
	}
}
