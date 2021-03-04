#include "UEngine.h"
#include "Material.h"

void UEngine::Material::Awake()
{
	colorBuffer->AttachData(&color, sizeof(Color));
	GameState::Get()->constantBufferPool.Add(colorBuffer);
}

void UEngine::Material::LateUpdate()
{
	auto renderComponent = GetComponent<RenderComponent>();
	if (renderComponent == nullptr) return;
	if (renderObject == nullptr || renderObject != renderComponent->GetRenderObject())
	{
		renderObject = renderComponent->GetRenderObject();
		renderComponent->AddConstantBuffer(typeid(this).raw_name(), colorBuffer);
		renderComponent->AddImageTexture(imageTexture);
	}
}

void UEngine::Material::OnPreRender()
{
	colorBuffer->Update(DXRenderer::Get()->GetImmediateDeviceContext());
}

void UEngine::Material::OnDestroy()
{
	GameState::Get()->constantBufferPool.Remove(colorBuffer);
	DXRenderer::DXTexture::Release(&imageTexture);
}

void UEngine::Material::Load(std::wstring fileName)
{
	imageTexture = DXRenderer::DXTexture::Load(fileName);
	color = Color{ 1, 1, 1, 1 };
	if (renderObject != nullptr)
		GetComponent<RenderComponent>()->AddImageTexture(imageTexture);
}
