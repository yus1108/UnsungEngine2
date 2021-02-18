#include "dxrframework.h"
#include "DXConstantBuffer.h"

UEngine::DXConstantBuffer* const UEngine::DXConstantBuffer::Instantiate(DXRenderer* const renderer, size_t _Data_Size)
{

	DXConstantBuffer* instnace = new DXConstantBuffer;
	renderer->InitConstantBuffer(_Data_Size, instnace->constBuffer.GetAddressOf());

	return instnace;
}

void UEngine::DXConstantBuffer::Release(DXConstantBuffer** const constantBuffer)
{
	delete* constantBuffer;
	*constantBuffer = nullptr;
}

void UEngine::DXConstantBuffer::UpdateBuffer(ID3D11DeviceContext* const deviceContext, const void* data, size_t _Size)
{
	// world matrix
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	deviceContext->Map(constBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &mappedResource);
	memcpy(mappedResource.pData, data, _Size);
	deviceContext->Unmap(constBuffer.Get(), 0);
}
