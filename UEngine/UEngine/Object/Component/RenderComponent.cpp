#include "UEngine.h"
#include "RenderComponent.h"

void UEngine::RenderComponent::OnPreRender()
{
	for (auto bufferPair : renderObject->constantBuffers)
	{
		bufferPair.second->Update()
	}
}

void UEngine::RenderComponent::Load(std::string renderMesh_name, std::string shader_name)
{
	if (renderObject != nullptr) GameState::Get()->gameScene.RemoveObject(renderObject);
	renderObject = nullptr;
	renderObject = GameState::Get()->gameScene.LoadObject(renderMesh_name, shader_name);
	if (GetGameObject()->GetActive() && GetEnable())
	{
		GameState::Get()->gameScene.AddObject(renderObject);
	}
}

void UEngine::RenderComponent::AddConstantBuffer(std::string type_raw_name, DXRenderer::DXConstantBuffer* buffer)
{
	renderObject->constantBuffers[type_raw_name] = buffer;
}
