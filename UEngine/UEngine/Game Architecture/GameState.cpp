#include "UEngine.h"
#include "GameState.h"

UEngine::GameState* UEngine::GameState::instance = nullptr;

bool UEngine::GameState::IsFixedUpdate()
{
	float timestep = GetCurrentFixedTimestep();
	instance->fixedUpdateTimer -= timestep;
	return instance->fixedUpdateTimer < timestep;
}

void UEngine::GameState::Init(GameScene* scene, bool drawOnBackBuffer)
{
	instance = new GameState;
	instance->isTerminate = false;
	instance->drawOnBackBuffer = drawOnBackBuffer;
	instance->currentScene = scene;
	instance->scenes[scene->name] = scene;
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
		instance->isTerminate = true;
	}
}

void UEngine::GameState::Release()
{
	instance->isTerminate = true;
	for (auto scene : instance->scenes)
		delete scene.second;
	instance->scenes.clear();
	instance->currentScene = nullptr;
	delete instance;
	instance = nullptr;
}
