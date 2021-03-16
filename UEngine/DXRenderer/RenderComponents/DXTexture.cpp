#include "dxrframework.h"
#include "DXTexture.h"

void UEngine::DXRenderer::DXTexture::Set(ID3D11DeviceContext* const deviceContext, UINT startSlot)
{
	deviceContext->PSSetShaderResources(startSlot, 1, GetTextureAddressOf());
}


UEngine::DXRenderer::DXTexture* UEngine::DXRenderer::DXTexture::Load(std::wstring fileName)
{
	DXTexture* dxTexture = new DXTexture;
	dxTexture->fileName = fileName;
	auto hr = CreateWICTextureFromFile(
		DXRenderer::Get()->GetDevice(), 
		DXRenderer::Get()->GetTextureLoadDeviceContext(),
		dxTexture->fileName.c_str(),
		(ID3D11Resource**)dxTexture->texture.GetAddressOf(), 
		dxTexture->shader_resource_view.GetAddressOf()
	);
	if (dxTexture->shader_resource_view.Get() == nullptr) {
		CreateDDSTextureFromFile(DXRenderer::Get()->GetDevice(), dxTexture->fileName.c_str(),
			(ID3D11Resource**)dxTexture->texture.GetAddressOf(), dxTexture->shader_resource_view.GetAddressOf());
		if (dxTexture->shader_resource_view.Get() == nullptr) throw std::runtime_error("Fail to load texture");
	}
	return dxTexture;
}

void UEngine::DXRenderer::DXTexture::Release(DXTexture* const texture)
{
	delete texture;
}
