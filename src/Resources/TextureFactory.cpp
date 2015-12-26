#include "Resources/TextureFactory.hpp"

#include "Import/tga/ImportTga.hpp"
#include "Macros/Macros.hpp"
#include "d3d11/D3D11Texture.hpp"

using namespace Eternal::Resources;
using namespace Eternal::Import;
using namespace Eternal::Graphics;

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

Texture* TextureFactory::GetTexture(_In_ const string& NameSrc)
{
	map<string, TextureCache>::iterator FoundTexture = _Textures.find(NameSrc);
	if (FoundTexture != _Textures.cend())
	{
		return FoundTexture->second.TextureObj;
	}
	uint32_t Height, Width;
	uint8_t* TextureData = ImportTga::Get()->Import(NameSrc, Height, Width);

	TextureCache& TextureCacheLine = _Textures[NameSrc];
	TextureCacheLine.TextureObj = new Graphics::D3D11Texture(Graphics::Texture::BGRA8888, Graphics::D3D11Resource::DYNAMIC, Graphics::Resource::WRITE, Width, Height, TextureData);

	return TextureCacheLine.TextureObj;
}
