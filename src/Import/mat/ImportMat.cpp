#include "Import/mat/ImportMat.hpp"
#include "Resources/MeshFactory.hpp"
#include "File/FilePath.hpp"
#include "File/File.hpp"
#include "File/FileFactory.hpp"
#include "Log/Log.hpp"
#include "rapidjson/document.h"

namespace Eternal
{
	namespace Import
	{
		using namespace std;
		using namespace Eternal::FileSystem;
		using namespace Eternal::Resources;
		using namespace Eternal::Components;

		ImportMat::ImportMat()
		{

		}

		Components::Material* ImportMat::Import(_In_ const std::string& InPath, _Inout_ Resources::MaterialDependency& InOutMaterialDependency)
		{
			Components::Material* OutMaterial = new Components::Material();

			string FullPath = FilePath::Find(string(InPath) + ".mat", FileType::FILE_TYPE_MATERIALS);
			if (FullPath.size() > 0)
			{
				FileContent MaterialContent = LoadFileToMemory(FullPath);

				auto Entry = InOutMaterialDependency.Textures.emplace(OutMaterial, MaterialTextures()).first;
				rapidjson::Document JsonMaterial;
				JsonMaterial.Parse(reinterpret_cast<const char*>(MaterialContent.Content), MaterialContent.Size);

				auto& JsonMaterialEntry = JsonMaterial["material"];
				for (uint32_t TextureTypeIndex = 0; TextureTypeIndex < static_cast<uint32_t>(TextureType::TEXTURE_TYPE_COUNT); ++TextureTypeIndex)
				{
					MaterialDependencyEntry& Textures = Entry->second.Textures[TextureTypeIndex];
					if (TextureTypeIndex < JsonMaterialEntry.Size())
					{
						Textures.TextureKey		= JsonMaterialEntry[TextureTypeIndex].GetString();
						Textures.TexturePath	= JsonMaterialEntry[TextureTypeIndex].GetString();
					}
					else
					{
						Textures.TextureKey		= "black";
						Textures.TexturePath	= "black.tga";
					}
				}

				auto& JsonMaterialTypeEntry = JsonMaterial["material_type"];
				OutMaterial->SetMaterialType(static_cast<MaterialType>(JsonMaterialTypeEntry.GetInt()));
			}
			else
			{
				LogWrite(LogWarning, LogImport, string("[ImportMat::Import] Couldn't import: ") + string(InPath) + ".mat");
			}

			return OutMaterial;
		}
	}
}
