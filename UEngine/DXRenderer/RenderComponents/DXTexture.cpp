#include "dxrframework.h"
#include "DXTexture.h"

UEngine::DXRenderer::DXTexture* UEngine::DXRenderer::DXTexture::Load(std::wstring fileName)
{
	DXTexture* dxTexture = new DXTexture;
	dxTexture->fileName = fileName;
	auto hr = CreateWICTextureFromFile(
		DXRenderer::Get()->GetDevice(), 
		DXRenderer::Get()->GetTextureLoadDeviceContext(),
		fileName.c_str(), 
		(ID3D11Resource**)dxTexture->texture.GetAddressOf(), 
		dxTexture->shader_resource_view.GetAddressOf()
	);
	if (dxTexture->shader_resource_view.Get() == nullptr) {
		CreateDDSTextureFromFile(DXRenderer::Get()->GetDevice(), fileName.c_str(),
			(ID3D11Resource**)dxTexture->texture.GetAddressOf(), dxTexture->shader_resource_view.GetAddressOf());
		if (dxTexture->shader_resource_view.Get() == nullptr) throw std::runtime_error("Fail to load texture");
	}
	return dxTexture;
}

void UEngine::DXRenderer::DXTexture::Release(DXTexture** const texture)
{
	delete *texture;
	*texture = nullptr;
}
