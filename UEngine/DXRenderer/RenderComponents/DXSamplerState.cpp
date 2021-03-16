#include "dxrframework.h"
#include "DXSamplerState.h"

void UEngine::DXRenderer::DXSamplerState::Set(ID3D11DeviceContext* const deviceContext, UINT startSlot)
{
	deviceContext->PSSetSamplers(startSlot, 1, samplerState.GetAddressOf());
}

UEngine::DXRenderer::DXSamplerState* UEngine::DXRenderer::DXSamplerState::Load(D3D11_SAMPLER_DESC desc)
{
	DXSamplerState* dxSamplerState = new DXSamplerState;
	dxSamplerState->fileName = MakeName(desc);
	DXRenderer::Get()->GetDevice()->CreateSamplerState(&desc, dxSamplerState->samplerState.GetAddressOf());
	return dxSamplerState;
}

std::string UEngine::DXRenderer::DXSamplerState::MakeName(D3D11_SAMPLER_DESC desc)
{
	using namespace std;
	std::string name;
	name = to_string(desc.AddressU) + "\n" + to_string(desc.AddressV) + "\n" + to_string(desc.AddressW) + "\n";
	name += to_string(desc.BorderColor[0]) + "\n" + to_string(desc.BorderColor[1]) + "\n" + to_string(desc.BorderColor[2]) + "\n" + to_string(desc.BorderColor[3]) + "\n";
	name += to_string(desc.ComparisonFunc) + "\n" + to_string(desc.Filter) + "\n" + to_string(desc.MaxAnisotropy) + "\n";
	name += to_string(desc.MaxLOD) + "\n" + to_string(desc.MinLOD) + "\n" + to_string(desc.MipLODBias) + "\n";
	return name;
}

void UEngine::DXRenderer::DXSamplerState::Release(DXSamplerState* const texture)
{
	delete texture;
}
