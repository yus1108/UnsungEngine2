#include "UEngine.h"
#include "Material.h"

void UEngine::Material::Awake()
{
	colorBuffer->AttachData(&color, sizeof(Color));
	spriteBuffer->AttachData(&uv, sizeof(UV));
	GameState::Get()->constantBufferPool.Add(colorBuffer);
	GameState::Get()->constantBufferPool.Add(spriteBuffer);
}

void UEngine::Material::LateUpdate()
{
	auto renderComponent = GetComponent<RenderComponent>();
	if (renderComponent == nullptr) return;
	if (renderObject == nullptr || renderObject != renderComponent->GetRenderObject())
	{
		renderObject = renderComponent->GetRenderObject();
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
	GameState::Get()->constantBufferPool.Remove(colorBuffer);
	GameState::Get()->constantBufferPool.Remove(spriteBuffer);
	DXRenderer::DXTexture::Release(&imageTexture);
}

void UEngine::Material::Load(std::wstring fileName)
{
	imageTexture = DXRenderer::DXTexture::Load(fileName);
	color = Color{ 1, 1, 1, 1 };
	uv = UV{ 0, 0, 1, 1 };
	if (renderObject != nullptr)
		GetComponent<RenderComponent>()->AddImageTexture(imageTexture);
}
