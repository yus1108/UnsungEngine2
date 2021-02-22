#include "dxrframework.h"
#include "DXConstantBuffer.h"

namespace UEngine
{
	namespace DXRenderer
	{
		DXConstantBuffer* DXConstantBuffer::Instantiate
		(
			DXRenderer* const renderer,
			size_t _Data_Size,
			UINT flags,
			UINT startSlot
		)
		{
			DXConstantBuffer* instance = new DXConstantBuffer;
			instance->StartSlot = 0;
			instance->Flags = flags;
			renderer->InitConstantBuffer(_Data_Size, instance->constBuffer.GetAddressOf());

			return instance;
		}

		void DXConstantBuffer::Release(DXConstantBuffer** const constantBuffer)
		{
			delete* constantBuffer;
			*constantBuffer = nullptr;
		}

		void DXConstantBuffer::AttachData(const void* data, size_t _Size)
		{
			if (!attached) delete this->data;
			this->data = const_cast<void*>(data);
			this->_Size = _Size;
			attached = true;
		}

		void DXConstantBuffer::Update(ID3D11DeviceContext* const deviceContext)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
			deviceContext->Map(constBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &mappedResource);
			memcpy(mappedResource.pData, data, _Size);
			deviceContext->Unmap(constBuffer.Get(), 0);
		}

		void DXConstantBuffer::Set(ID3D11DeviceContext* const deviceContext)
		{
			if (Flags & UENGINE_DXSHADERTYPE_VERTEX_SHADER) deviceContext->VSSetConstantBuffers(StartSlot, 1, constBuffer.GetAddressOf());
			if (Flags & UENGINE_DXSHADERTYPE_PIXEL_SHADER) deviceContext->PSSetConstantBuffers(StartSlot, 1, constBuffer.GetAddressOf());
			if (Flags & UENGINE_DXSHADERTYPE_GEOMETRY_SHADER) deviceContext->GSSetConstantBuffers(StartSlot, 1, constBuffer.GetAddressOf());
			if (Flags & UENGINE_DXSHADERTYPE_HULL_SHADER) deviceContext->DSSetConstantBuffers(StartSlot, 1, constBuffer.GetAddressOf());
			if (Flags & UENGINE_DXSHADERTYPE_DOMAIN_SHADER) deviceContext->HSSetConstantBuffers(StartSlot, 1, constBuffer.GetAddressOf());
		}

	}
}
