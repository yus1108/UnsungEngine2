#include "SceneScript.h"

void SceneScript::Load(UEngine::GameState* gameState)
{
	using namespace UEngine;

	// basic load
	{
		GameObject* mainCamera = GameObject::Instantiate();
		mainCamera->AddComponent<Transform>();
		mainCamera->AddComponent<Camera>()->SetMainCamera();
		GameObject* rectangle = GameObject::Instantiate();
		rectangle->AddComponent<Transform>();
		rectangle->AddComponent<RenderComponent>()->Load("rectangle", "color");
		rectangle->AddComponent<Material>();

		gameState->AddObject(mainCamera);
		gameState->AddObject(rectangle);
	}
	
}
