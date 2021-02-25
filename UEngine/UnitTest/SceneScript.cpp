#include "SceneScript.h"

void SceneScript::Load(UEngine::GameState* gameState)
{
	using namespace UEngine;

	// basic load
	{
		GameObject* mainCamera = GameObject::Instantiate();
		mainCamera->AddComponent<Transform>();
		mainCamera->AddComponent<Camera>();
		GameObject* rectangle = GameObject::Instantiate();
		rectangle->AddComponent<Transform>();
		rectangle->AddComponent<RenderComponent>()->Load("rectangle", "color");
		rectangle->AddComponent<Material>()->color = Color{ 1, 0, 0, 1 };

		gameState->AddObject(mainCamera);
		gameState->AddObject(rectangle);
	}
	
}
