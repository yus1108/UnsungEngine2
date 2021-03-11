#include "UEngine.h"
#include "GameState.h"

UEngine::GameState UEngine::GameState::instance;

void UEngine::GameState::Release()
{
	isTerminate = true;
	for (auto scene : scenes)
		delete scene.second;
	scenes.clear();
	currentScene = nullptr;
}
