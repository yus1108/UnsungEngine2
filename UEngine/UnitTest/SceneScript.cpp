#include "SceneScript.h"

void SceneScript::Load(UEngine::GameState* gameState)
{
	using namespace UEngine;

	// basic load
	{
		GameObject* mainCamera = GameObject::Instantiate();
		mainCamera->AddComponent<Camera>()->SetMainCamera();
		GameObject* rectangle = GameObject::Instantiate();
		rectangle->AddComponent<RenderComponent>();
		rectangle->AddComponent<Material>();

		gameState->AddObject(mainCamera);
		gameState->AddObject(rectangle);
	}
	
}
