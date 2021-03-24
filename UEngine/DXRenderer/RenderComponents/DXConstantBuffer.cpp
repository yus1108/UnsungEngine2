#include "dxrframework.h"
#include "DXConstantBuffer.h"
#include <typeinfo>

namespace UEngine
{
	namespace DXRenderer
	{
		DXConstantBuffer* DXConstantBuffer::Instantiate
		(
			std::string UID,
			DXRenderer* const renderer,
			CONSTANT_BUFFER_DESC desc
		)
		{
			DXConstantBuffer* instance = new DXConstantBuffer;
			instance->UID = UID;
			instance->StartSlots = desc.StartSlots;
			instance->Flags = desc.Flag;
			renderer->InitConstantBuffer(desc.Size, instance->constBuffer.GetAddressOf());
			return instance;
		}

		void DXConstantBuffer::Release(DXConstantBuffer** const constantBuffer)
		{
			delete* constantBuffer;
			*constantBuffer = nullptr;
		}

		void DXConstantBuffer::Release(DXConstantBuffer* const constantBuffer)
		{
			delete constantBuffer;
		}

		void DXConstantBuffer::AttachData(const void* data, size_t Size)
		{
			if (!attached) delete this->data;
			this->data = const_cast<void*>(data);
			this->Size = Size;
			attached = true;
		}

		void DXConstantBuffer::Update(ID3D11DeviceContext* const deviceContext)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
			deviceContext->Map(constBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &mappedResource);
			memcpy(mappedResource.pData, data, Size);
			deviceContext->Unmap(constBuffer.Get(), 0);
		}

		void DXConstantBuffer::Set(ID3D11DeviceContext* const deviceContext)
		{
			if (StartSlots == nullptr)
			{
				if (Flags & UENGINE_DXRENDERER_SHADERTYPE_VERTEX_SHADER) deviceContext->VSSetConstantBuffers(0, 1, constBuffer.GetAddressOf());
				if (Flags & UENGINE_DXRENDERER_SHADERTYPE_PIXEL_SHADER) deviceContext->PSSetConstantBuffers(0, 1, constBuffer.GetAddressOf());
				if (Flags & UENGINE_DXRENDERER_SHADERTYPE_GEOMETRY_SHADER) deviceContext->GSSetConstantBuffers(0, 1, constBuffer.GetAddressOf());
				if (Flags & UENGINE_DXRENDERER_SHADERTYPE_HULL_SHADER) deviceContext->DSSetConstantBuffers(0, 1, constBuffer.GetAddressOf());
				if (Flags & UENGINE_DXRENDERER_SHADERTYPE_DOMAIN_SHADER) deviceContext->HSSetConstantBuffers(0, 1, constBuffer.GetAddressOf());
				return;
			}

			UINT index = 0;
			if (Flags & UENGINE_DXRENDERER_SHADERTYPE_VERTEX_SHADER) deviceContext->VSSetConstantBuffers(StartSlots[index++], 1, constBuffer.GetAddressOf());
			if (Flags & UENGINE_DXRENDERER_SHADERTYPE_PIXEL_SHADER) deviceContext->PSSetConstantBuffers(StartSlots[index++], 1, constBuffer.GetAddressOf());
			if (Flags & UENGINE_DXRENDERER_SHADERTYPE_GEOMETRY_SHADER) deviceContext->GSSetConstantBuffers(StartSlots[index++], 1, constBuffer.GetAddressOf());
			if (Flags & UENGINE_DXRENDERER_SHADERTYPE_HULL_SHADER) deviceContext->DSSetConstantBuffers(StartSlots[index++], 1, constBuffer.GetAddressOf());
			if (Flags & UENGINE_DXRENDERER_SHADERTYPE_DOMAIN_SHADER) deviceContext->HSSetConstantBuffers(StartSlots[index++], 1, constBuffer.GetAddressOf());
		}

	}
}
