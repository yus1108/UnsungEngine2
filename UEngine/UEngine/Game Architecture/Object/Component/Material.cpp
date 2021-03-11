#include "UEngine.h"
#include "Material.h"

void UEngine::Material::Awake()
{
	colorBuffer->AttachData(&color, sizeof(Color));
	spriteBuffer->AttachData(&uv, sizeof(UV));
	GetGameObject()->GetScene()->ResourceManager.AddResource<DXRenderer::DXConstantBuffer>(colorBuffer->UID, colorBuffer);
	GetGameObject()->GetScene()->ResourceManager.AddResource<DXRenderer::DXConstantBuffer>(spriteBuffer->UID, spriteBuffer);
}

void UEngine::Material::LateUpdate()
{
	auto renderComponent = GetComponent<RenderComponent>();
	if (renderComponent == nullptr) return;
	if (renderObject != renderComponent->GetRenderObject())
	{
		renderObject = renderComponent->GetRenderObject();
		if (renderObject == nullptr) return;
		renderComponent->AddConstantBuffer(typeid(Color).raw_name(), colorBuffer);
		renderComponent->AddConstantBuffer(typeid(UV).raw_name(), spriteBuffer);
		renderComponent->AddImageTexture(imageTexture);
	}
}

void UEngine::Material::OnPreRender()
{
	colorBuffer->Update(DXRenderer::Get()->GetImmediateDeviceContext());
	spriteBuffer->Update(DXRenderer::Get()->GetImmediateDeviceContext());
}

void UEngine::Material::OnDestroy()
{
	GetGameObject()->GetScene()->ResourceManager.RemoveResource<DXRenderer::DXConstantBuffer>(colorBuffer->UID);
	GetGameObject()->GetScene()->ResourceManager.RemoveResource<DXRenderer::DXConstantBuffer>(spriteBuffer->UID);
	DXRenderer::DXTexture::Release(&imageTexture);
}

void UEngine::Material::Load(std::wstring fileName)
{
	// Sampler State
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.MaxLOD = FLT_MAX;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MipLODBias = 0;

	imageTexture = DXRenderer::DXTexture::Load(fileName, samplerDesc);
	color = Color{ 1, 1, 1, 1 };
	uv = UV{ 0, 0, 1, 1 };
	if (renderObject != nullptr)
		GetComponent<RenderComponent>()->AddImageTexture(imageTexture);
}

void UEngine::Material::Load(std::wstring fileName, D3D11_TEXTURE_ADDRESS_MODE addressMode, D3D11_FILTER filter)
{
	// Sampler State
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.AddressU = addressMode;
	samplerDesc.AddressV = addressMode;
	samplerDesc.AddressW = addressMode;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.Filter = filter;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.MaxLOD = FLT_MAX;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MipLODBias = 0;

	imageTexture = DXRenderer::DXTexture::Load(fileName, samplerDesc);
	color = Color{ 1, 1, 1, 1 };
	uv = UV{ 0, 0, 1, 1 };
	if (renderObject != nullptr)
		GetComponent<RenderComponent>()->AddImageTexture(imageTexture);
}
