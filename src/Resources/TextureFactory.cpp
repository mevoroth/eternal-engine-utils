#include "Resources/TextureFactory.hpp"

#include "Import/tga/ImportTga.hpp"
#include "Macros/Macros.hpp"

using namespace Eternal::Resources;
using namespace Eternal::Import;

TextureFactory* TextureFactory::_Inst = nullptr;

TextureFactory::TextureFactory()
{
	ETERNAL_ASSERT(!_Inst);
	_Inst = this;
}

TextureFactory* TextureFactory::Get()
{
	ETERNAL_ASSERT(_Inst);
	return _Inst;
}

uint8_t* TextureFactory::GetTexture(_In_ const string& NameSrc, _Out_ uint32_t& Height, _Out_ uint32_t& Width)
{
	map<string, TextureCache>::iterator FoundTexture = _Textures.find(NameSrc);
	if (FoundTexture != _Textures.cend())
	{
		Height = FoundTexture->second.Height;
		Width = FoundTexture->second.Width;
		return FoundTexture->second.TextureData;
	}
	uint8_t* TextureData = ImportTga::Get()->Import(NameSrc, Height, Width);
	
	TextureCache& TextureCacheLine = _Textures[NameSrc];
	TextureCacheLine.TextureData = TextureData;
	TextureCacheLine.Height = Height;
	TextureCacheLine.Width = Width;

	return TextureData;
}
