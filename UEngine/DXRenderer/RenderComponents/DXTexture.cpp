#include "dxrframework.h"
#include "DXTexture.h"

void UEngine::DXRenderer::DXTexture::Set(ID3D11DeviceContext* const deviceContext)
{
	deviceContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());
	deviceContext->PSSetShaderResources(0, 1, GetTextureAddressOf());
}

UEngine::DXRenderer::DXTexture* UEngine::DXRenderer::DXTexture::Load(std::wstring fileName, D3D11_SAMPLER_DESC samplerDesc)
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

	// Sampler State
	DXRenderer::Get()->GetDevice()->CreateSamplerState(&samplerDesc, dxTexture->samplerState.GetAddressOf());
	return dxTexture;
}

void UEngine::DXRenderer::DXTexture::Release(DXTexture** const texture)
{
	delete *texture;
	*texture = nullptr;
}

void UEngine::DXRenderer::DXTexture::Release(DXTexture* const texture)
{
	delete texture;
}
