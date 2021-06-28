#include "Resources/ImmediateTextureFactory.hpp"

#include "File/FileFactory.hpp"
#include "Import/tga/ImportTga.hpp"
#include "Graphics/Resource.hpp"
#include "Graphics/ResourceFactory.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Graphics/CommandQueue.hpp"
#include "Graphics/Format.hpp"
#include "Graphics/CommandList.hpp"
#include <string>

namespace Eternal
{
	namespace Resources
	{
		bool ImmediateTextureFactoryLoadTextureCallback::LoadTexture(_In_ const string& Path, _Out_ RawTextureData& OutTextureData)
		{
			using namespace Eternal::File;
			using namespace Eternal::Import;

			if (FileExists(Path))
			{
				uint32_t Height	= 0;
				uint32_t Width	= 0;
				uint8_t* ImageData = ImportTga::Get()->Import(Path, Height, Width);

				OutTextureData.Initialize(ImageData, Width, Height, 1, 4);
				return true;
			}

			return false;
		}

		ImmediateTextureFactoryCreateGpuResourceCallback::ImmediateTextureFactoryCreateGpuResourceCallback(_In_ GraphicsContext& InContext)
			: _Context(InContext)
		{
		}

		void ImmediateTextureFactoryCreateGpuResourceCallback::BeginBatch()
		{
			ETERNAL_ASSERT(!_CommandList);
			_CommandList = _Context.CreateNewCommandList(CommandType::COMMAND_TYPE_GRAPHIC);
			_CommandList->Begin(_Context);

		}

		void ImmediateTextureFactoryCreateGpuResourceCallback::EndBatch()
		{
			ETERNAL_ASSERT(_CommandList);
			_CommandList->End();
			_Context.GetGraphicsQueue().SubmitCommandLists(
				&_CommandList, 1
			);
			_CommandList = nullptr;
		}

		bool ImmediateTextureFactoryCreateGpuResourceCallback::CreateTextureResource(_In_ const string& InName, _Inout_ RawTextureData& InOutTextureData, _Out_ Resource*& OutTexture)
		{
			using namespace Eternal::Graphics;

			//////////////////////////////////////////////////////////////////////////
			// CPU Buffer
			std::string UploadBufferName = "AnonymousUploadBuffer";
			BufferResourceCreateInformation UploadBufferCreateInformation(
				_Context.GetDevice(),
				UploadBufferName,
				BufferCreateInformation(
					Format::FORMAT_RGBA8888,
					ResourceUsage::RESOURCE_USAGE_NONE,
					InOutTextureData.Width * InOutTextureData.Height * InOutTextureData.DepthOrArraySize * InOutTextureData.Stride
				),
				ResourceMemoryType::RESOURCE_MEMORY_TYPE_GPU_UPLOAD
			);

			Resource* UploadBuffer = CreateBuffer(UploadBufferCreateInformation);

			//////////////////////////////////////////////////////////////////////////
			// GPU Texture
			//_In_ const ResourceDimension& InResourceDimension,
			//	_In_ const Format& InFormat,
			//	_In_ const ResourceUsage& InResourceUsage,
			//	_In_ uint32_t InWidth,
			//	_In_ uint32_t InHeight = 1,
			//	_In_ uint32_t InDepthOrArraySize = 1,
			//	_In_ uint32_t InMIPLevels = 1,
			//	_In_ const float InClearValue[ComponentCount] = DefaultClearValue

			TextureResourceCreateInformation GPUTextureCreateInformation(
				_Context.GetDevice(),
				InName,
				TextureCreateInformation(
					ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D,
					Format::FORMAT_RGBA8888,
					ResourceUsage::RESOURCE_USAGE_SHADER_RESOURCE,
					InOutTextureData.Width,
					InOutTextureData.Height,
					InOutTextureData.DepthOrArraySize,
					1
				),
				ResourceMemoryType::RESOURCE_MEMORY_TYPE_GPU_MEMORY,
				TransitionState::TRANSITION_COPY_WRITE
			);
			OutTexture = CreateTexture(GPUTextureCreateInformation);

			ResourceTransition TextureCopyWriteToShaderResource(
				OutTexture,
				TransitionState::TRANSITION_PIXEL_SHADER_READ
			);
			_CommandList->Transition(
				&TextureCopyWriteToShaderResource, 1
			);

			//vk::ImageMemoryBarrier TextureMemoryBarrier(
			//	vk::AccessFlagBits(),
			//	vk::AccessFlagBits::eTransferWrite,
			//	vk::ImageLayout::eUndefined,
			//	vk::ImageLayout::eTransferDstOptimal,
			//	0, 0,
			//	Texture,
			//	vk::ImageSubresourceRange(
			//		vk::ImageAspectFlagBits::eColor,
			//		0, 1,
			//		0, 1
			//	)
			//);

			//CmdList.pipelineBarrier(
			//	vk::PipelineStageFlagBits::eHost,
			//	vk::PipelineStageFlagBits::eTransfer,
			//	vk::DependencyFlagBits(),
			//	0, static_cast<vk::MemoryBarrier*>(nullptr),
			//	0, static_cast<vk::BufferMemoryBarrier*>(nullptr),
			//	1, &TextureMemoryBarrier
			//);

			//vk::BufferImageCopy TextureRegion(
			//	0ull, TextureData.Width, TextureData.Height,
			//	vk::ImageSubresourceLayers(
			//		vk::ImageAspectFlagBits::eColor,
			//		0, 0, 1
			//	),
			//	vk::Offset3D(),
			//	vk::Extent3D(TextureData.Width, TextureData.Height, TextureData.Depth)
			//);

			//CmdList.copyBufferToImage(
			//	StagingBuffer,
			//	Texture,
			//	vk::ImageLayout::eTransferDstOptimal,
			//	1, &TextureRegion
			//);

			//TextureMemoryBarrier.setSrcAccessMask(vk::AccessFlagBits::eTransferWrite);
			//TextureMemoryBarrier.setDstAccessMask(vk::AccessFlagBits::eShaderRead);
			//TextureMemoryBarrier.setOldLayout(vk::ImageLayout::eTransferDstOptimal);
			//TextureMemoryBarrier.setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal);

			//CmdList.pipelineBarrier(
			//	vk::PipelineStageFlagBits::eTransfer,
			//	vk::PipelineStageFlagBits::eFragmentShader,
			//	vk::DependencyFlagBits(),
			//	0, static_cast<vk::MemoryBarrier*>(nullptr),
			//	0, static_cast<vk::BufferMemoryBarrier*>(nullptr),
			//	1, &TextureMemoryBarrier
			//);

			////// Clean up staging resources
			////vkFreeMemory(get_device().get_handle(), staging_memory, nullptr);
			////vkDestroyBuffer(get_device().get_handle(), staging_buffer, nullptr);

			//OutTexture = reinterpret_cast<Eternal::Graphics::Resource*>(&Texture);

			return true;
		}
	}
}
