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
	}
}

void UEngine::Material::OnPreRender()
{
	colorBuffer->Update(DXRenderer::Get()->GetImmediateDeviceContext());
}
