#include "UEngine.h"
#include "GameState.h"

UEngine::GameState UEngine::GameState::instance;

void UEngine::GameState::Init(GameScene* scene)
{
	instance.isTerminate = false;
	instance.currentScene = scene;
	instance.scenes[scene->name] = scene;
}

void UEngine::GameState::Release()
{
	instance.isTerminate = true;
	for (auto scene : instance.scenes)
		delete scene.second;
	instance.scenes.clear();
	instance.currentScene = nullptr;
}
