#include "UEngine.h"
#include "RenderComponent.h"

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
