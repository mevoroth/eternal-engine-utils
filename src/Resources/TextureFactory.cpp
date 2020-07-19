#include "Resources/TextureFactory.hpp"

#include "Import/tga/ImportTga.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/TextureFactory.hpp"
#include "Graphics/Format.hpp"
#include "File/FileFactory.hpp"
#include "File/FilePath.hpp"
#include "Parallel/MutexFactory.hpp"
#include "Parallel/Mutex.hpp"

using namespace Eternal::Resources;
using namespace Eternal::Import;
using namespace Eternal::Graphics;
using namespace Eternal::File;

TextureFactory* TextureFactory::_Inst = nullptr;
vector<string> TextureFactory::_IncludePaths;

void RawTextureData::Initialize(void* InTextureData)
{
	TextureData = InTextureData;
}

void RawTextureData::Release()
{
	delete[] TextureData;
	TextureData = nullptr;
}

TextureFactory::TextureFactory(_In_ const TextureFactoryCreateInformation& CreateInformation)
	: Callbacks(CreateInformation)
	, _NewRequestsMutex(CreateMutex())
{
	ETERNAL_ASSERT(!_Inst);

	//uint32_t Color = 0xFFFFFFFF;
	//_InsertNewTexture("_WHITE", CreateTexture(Graphics::BGRA8888, Resource::DYNAMIC, Resource::WRITE, 1, 1, &Color));

	//Color = 0xFFFF0000;
	//_InsertNewTexture("_RED", CreateTexture(Graphics::BGRA8888, Resource::DYNAMIC, Resource::WRITE, 1, 1, &Color));

	//Color = 0xFF000000;
	//_InsertNewTexture("_BLACK", CreateTexture(Graphics::BGRA8888, Resource::DYNAMIC, Resource::WRITE, 1, 1, &Color));

	//Color = 0xFF7F7FFF;
	//_InsertNewTexture("_DEFAULT_NORMAL", CreateTexture(Graphics::BGRA8888, Resource::DYNAMIC, Resource::WRITE, 1, 1, &Color));

	_Inst = this;
}

TextureFactory* TextureFactory::Get()
{
	ETERNAL_ASSERT(_Inst);
	return _Inst;
}

void TextureFactory::CreateRequest(const TextureFactoryRequest& Request)
{
	_NewRequestsMutex->Lock();
	_NewRequests.push_back(Request);
	_NewRequestsMutex->Unlock();
}

void TextureFactory::ProcessRequests()
{
	if (_NewRequestsMutex->TryLock())
	{
		_PendingLoadTextureRequests.insert(_PendingLoadTextureRequests.end(), _NewRequests.begin(), _NewRequests.end());
		_NewRequests.clear();
		_NewRequestsMutex->Unlock();
	}

	for (auto PendingRequestIterator = _PendingLoadTextureRequests.begin(); PendingRequestIterator != _PendingLoadTextureRequests.end(); )
	{
		TextureFactoryLoadedTexture LoadedTexture = *PendingRequestIterator;

		if (Callbacks.LoadTextureCallback.LoadTexture(LoadedTexture.Path, LoadedTexture.TextureData))
		{
			PendingRequestIterator = _PendingLoadTextureRequests.erase(PendingRequestIterator);
			_PendingCreateGpuResourceRequests.push_back(LoadedTexture);
		}
		else
		{
			++PendingRequestIterator;
		}
	}

	for (auto PendingRequestIterator = _PendingCreateGpuResourceRequests.begin(); PendingRequestIterator != _PendingCreateGpuResourceRequests.end(); )
	{
		Texture* OutTexture;
		if (Callbacks.CreateGpuResourceCallback.CreateTexture(PendingRequestIterator->TextureData, OutTexture))
		{
			_InsertNewTexture(PendingRequestIterator->Name, OutTexture);
			PendingRequestIterator = _PendingCreateGpuResourceRequests.erase(PendingRequestIterator);
		}
		else
		{
			++PendingRequestIterator;
		}
	}
}

bool TextureFactory::HasRequests() const
{
	return (_NewRequests.size() + _PendingLoadTextureRequests.size() + _PendingCreateGpuResourceRequests.size()) > 0;
}

void TextureFactory::RegisterTexturePath(_In_ const string& Path)
{
	_IncludePaths.push_back(Path);
}

Texture* TextureFactory::GetTexture(_In_ const string& NameSrc)
{
	map<string, TextureCache>::iterator FoundTexture = _Textures.find(NameSrc);
	if (FoundTexture != _Textures.cend())
	{
		return FoundTexture->second.TextureObj;
	}

	return nullptr;

	//std::string FilePath;
	//bool PathFound = false;
	//for (int IncludePathIndex = 0; !PathFound && IncludePathIndex < _IncludePaths.size(); ++IncludePathIndex)
	//{
	//	FilePath = _IncludePaths[IncludePathIndex] + NameSrc;
	//	PathFound = FileExists(FilePath);
	//}

	////ETERNAL_ASSERT(PathFound);
	//if (!PathFound)
	//	return nullptr;

	//uint32_t Height, Width;
	//uint8_t* TextureData = ImportTga::Get()->Import(FilePath, Height, Width);

	//TextureCache& TextureCacheLine = _Textures[NameSrc];
	//ETERNAL_ASSERT(false);
	//TextureCacheLine.TextureObj = CreateTexture(Graphics::BGRA8888, Resource::DYNAMIC, Resource::WRITE, Width, Height, TextureData);

	//return TextureCacheLine.TextureObj;
}

void TextureFactory::_InsertNewTexture(_In_ const string& NameSrc, _In_ Texture* TextureObj)
{
	map<string, TextureCache>::iterator FoundTexture = _Textures.find(NameSrc);
	ETERNAL_ASSERT(FoundTexture == _Textures.cend());

	TextureCache& TextureCacheLine = _Textures[NameSrc];
	TextureCacheLine.TextureObj = TextureObj;
}

TextureFactoryRequest::TextureFactoryRequest(const string& InName, const string& InPath)
	: Name(InName)
	, Path(FilePath::Find(InPath, FileType::TEXTURES))
{
}
