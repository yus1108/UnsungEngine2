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
	instance->windowSize = WinApplication::Get()->GetClientPixelSize();
}

void UEngine::GameState::Init(GameScene* scene, bool drawOnBackBuffer)
{
	if (instance == nullptr) instance = new GameState;
	instance->isTerminate = false;
	instance->drawOnBackBuffer = drawOnBackBuffer;
	instance->currentScene = scene;
	instance->scenes[scene->name] = scene;
	instance->windowSize = WinApplication::Get()->GetClientPixelSize();
}

void UEngine::GameState::AddScene(GameScene* scene, bool setCurrentScene)
{
	instance->scenes[scene->name] = scene;
	if (setCurrentScene) instance->currentScene = scene;
}

void UEngine::GameState::Update(std::function<void()> OnSync, std::function<bool()> OnUpdate, std::function<void()> OnRender)
{
	std::lock_guard<std::mutex> lock(instance->noRenderMutex);
	instance->deltaTime = Utility::UTime::Get()->DeltaTimeF();
	instance->fixedUpdateTimer += instance->deltaTime;

	if (OnSync) OnSync();
	instance->currentScene->Sync();

	WinApplication::Get()->threadPool.AddSyncTask([&]()
	{
		instance->currentScene->Render(DXRenderer::Get()->GetImmediateDeviceContext());
		
		DXRenderer::Get()->Begin(DXRenderer::Get()->GetContext());

		if (instance->drawOnBackBuffer)
		{
			DXRenderer::Get()->Draw(DXRenderer::Get()->GetContext(), instance->currentScene->MainView->view->GetAddressOfViewResource());
			if (instance->currentScene->IsDebugMode() && instance->currentScene->debugRenderer)
				DXRenderer::Get()->Draw(DXRenderer::Get()->GetContext(), instance->currentScene->debugRenderer->GetAddressOfViewResource());
		}

		if (OnRender) OnRender();
		DXRenderer::Get()->End(nullptr);
	});

	if (OnUpdate) instance->isTerminate = OnUpdate();
	
	instance->currentScene->Update();
	WinApplication::Get()->threadPool.Join();


	instance->currentScene->PostRender();
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
