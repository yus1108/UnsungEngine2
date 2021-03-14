#include "UEngine.h"
#include "GameState.h"

UEngine::GameState* UEngine::GameState::instance = nullptr;

bool UEngine::GameState::IsFixedUpdate()
{
	float timestep = GetCurrentFixedTimestep();
	instance->fixedUpdateTimer -= timestep;
	return instance->fixedUpdateTimer < timestep;
}

void UEngine::GameState::SetCurrentScene(std::string name)
{
	auto scene = GetScene(name);
	if (!scene) throw std::runtime_error("Cannot set nullptr as the current scene of the game state");
	instance->currentScene = scene;
}

void UEngine::GameState::LoadScene(std::string fileName)
{

}

void UEngine::GameState::SaveScene(std::string fileName)
{

}

UEngine::GameScene* UEngine::GameState::GetScene(std::string name)
{
	auto iter = instance->scenes.find(name);
	if (iter == instance->scenes.end())
		return nullptr;
	return iter->second;
}

void UEngine::GameState::Init(bool drawOnBackBuffer)
{
	if (instance == nullptr) instance = new GameState;
	instance->isTerminate = false;
	instance->drawOnBackBuffer = drawOnBackBuffer;
}

void UEngine::GameState::Init(GameScene* scene, bool drawOnBackBuffer)
{
	if (instance == nullptr) instance = new GameState;
	instance->isTerminate = false;
	instance->drawOnBackBuffer = drawOnBackBuffer;
	instance->currentScene = scene;
	instance->scenes[scene->name] = scene;
}

void UEngine::GameState::AddScene(GameScene* scene, bool setCurrentScene)
{
	instance->scenes[scene->name] = scene;
	if (setCurrentScene) instance->currentScene = scene;
}

void UEngine::GameState::Update(std::function<bool()> OnUpdate, std::function<void()> OnRender)
{
	instance->deltaTime = Utility::UTime::Get()->DeltaTimeF();
	instance->fixedUpdateTimer += instance->deltaTime;

	instance->currentScene->Sync();

	WinApplication::Get()->threadPool.AddSyncTask([&]()
	{
		DXRenderer::Get()->Begin();
		instance->currentScene->Render(DXRenderer::Get()->GetImmediateDeviceContext());
		if (instance->drawOnBackBuffer)
		{
			DXRenderer::Get()->Draw(instance->currentScene->MainView->view->GetAddressOfViewResource());
			if (instance->currentScene->IsDebugMode() && instance->currentScene->debugRenderer)
				DXRenderer::Get()->Draw(instance->currentScene->debugRenderer->GetAddressOfViewResource());
		}
		if (OnRender) OnRender();
		DXRenderer::Get()->End();
	});

	bool run = false;
	if (OnUpdate) run = OnUpdate();
	
	instance->currentScene->Update();
	WinApplication::Get()->threadPool.Join();
	if (run)
	{
		//instance->isTerminate = true;
	}
}

void UEngine::GameState::Release()
{
	if (instance == nullptr) return;
	instance->isTerminate = true;
	for (auto scene : instance->scenes)
		delete scene.second;
	instance->scenes.clear();
	instance->currentScene = nullptr;
	delete instance;
	instance = nullptr;
}
