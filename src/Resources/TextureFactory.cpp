#include "Resources/TextureFactory.hpp"

#include "Import/tga/ImportTga.hpp"
#include "File/FileFactory.hpp"
#include "File/FilePath.hpp"
#include "Parallel/MutexFactory.hpp"
#include "Parallel/Mutex.hpp"

using namespace Eternal::Resources;
using namespace Eternal::Import;
using namespace Eternal::Graphics;
using namespace Eternal::File;

TextureFactory* TextureFactory::_Instance = nullptr;
vector<string> TextureFactory::_IncludePaths;

void RawTextureData::Initialize(
	_In_ void* InTextureData,
	_In_ uint32_t InWidth,
	_In_ uint32_t InHeight,
	_In_ uint32_t InDepthOrArraySize,
	_In_ uint32_t InStride
)
{
	TextureData			= InTextureData;
	Width				= InWidth;
	Height				= InHeight;
	DepthOrArraySize	= InDepthOrArraySize;
	Stride				= InStride;
}

void RawTextureData::Release()
{
	delete[] TextureData;
	TextureData = nullptr;
}

TextureFactory::TextureFactory(_In_ const TextureFactoryCreateInformation& InTextureFactoryCreateInformation)
	: Callbacks(InTextureFactoryCreateInformation)
	, _NewRequestsMutex(CreateMutex())
{
	ETERNAL_ASSERT(!_Instance);

	//uint32_t Color = 0xFFFFFFFF;
	//_InsertNewTexture("_WHITE", CreateTexture(Graphics::BGRA8888, Resource::DYNAMIC, Resource::WRITE, 1, 1, &Color));

	//Color = 0xFFFF0000;
	//_InsertNewTexture("_RED", CreateTexture(Graphics::BGRA8888, Resource::DYNAMIC, Resource::WRITE, 1, 1, &Color));

	//Color = 0xFF000000;
	//_InsertNewTexture("_BLACK", CreateTexture(Graphics::BGRA8888, Resource::DYNAMIC, Resource::WRITE, 1, 1, &Color));

	//Color = 0xFF7F7FFF;
	//_InsertNewTexture("_DEFAULT_NORMAL", CreateTexture(Graphics::BGRA8888, Resource::DYNAMIC, Resource::WRITE, 1, 1, &Color));

	_Instance = this;
}

TextureFactory* TextureFactory::Get()
{
	ETERNAL_ASSERT(_Instance);
	return _Instance;
}

void TextureFactory::CreateRequest(const TextureFactoryRequest& InRequest)
{
	_NewRequestsMutex->Lock();
	_NewRequests.push_back(InRequest);
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

	for (auto& PendingRequestIterator = _PendingLoadTextureRequests.begin(); PendingRequestIterator != _PendingLoadTextureRequests.end(); )
	{
		TextureFactoryLoadedTexture LoadedTexture(*PendingRequestIterator);

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

	if (_PendingCreateGpuResourceRequests.size())
	{
		Callbacks.CreateGpuResourceCallback.BeginBatch();
		for (auto& PendingRequestIterator = _PendingCreateGpuResourceRequests.begin(); PendingRequestIterator != _PendingCreateGpuResourceRequests.end(); )
		{
			Resource* OutTexture = nullptr;
			if (Callbacks.CreateGpuResourceCallback.CreateTextureResource(PendingRequestIterator->Name, PendingRequestIterator->TextureData, OutTexture))
			{
				_InsertNewTexture(PendingRequestIterator->Name, OutTexture);
				PendingRequestIterator->TextureData.Release();
				PendingRequestIterator = _PendingCreateGpuResourceRequests.erase(PendingRequestIterator);
			}
			else
			{
				++PendingRequestIterator;
			}
		}
		Callbacks.CreateGpuResourceCallback.EndBatch();
	}
}

bool TextureFactory::HasRequests() const
{
	return (_NewRequests.size() + _PendingLoadTextureRequests.size() + _PendingCreateGpuResourceRequests.size()) > 0;
}

void TextureFactory::RegisterTexturePath(_In_ const string& InPath)
{
	_IncludePaths.push_back(InPath);
}

Resource* TextureFactory::GetTexture(_In_ const string& InName)
{
	map<string, TextureCache>::iterator FoundTexture = _Textures.find(InName);
	if (FoundTexture != _Textures.cend())
	{
		return FoundTexture->second.CachedTexture;
	}

	return nullptr;
}

void TextureFactory::_InsertNewTexture(_In_ const string& InName, _In_ Resource* InTexture)
{
	map<string, TextureCache>::iterator FoundTexture = _Textures.find(InName);
	ETERNAL_ASSERT(FoundTexture == _Textures.cend());

	TextureCache& TextureCacheLine = _Textures[InName];
	TextureCacheLine.CachedTexture = InTexture;
}

TextureFactoryRequest::TextureFactoryRequest(const string& InName, const string& InPath)
	: Name(InName)
	, Path(FilePath::Find(InPath, FileType::TEXTURES))
{
}
