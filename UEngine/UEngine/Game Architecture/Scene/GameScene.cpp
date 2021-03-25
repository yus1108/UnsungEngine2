#include "UEngine.h"
#include "../../XMLParser/XMLSceneParser.h"
#include "GameScene.h"

void UEngine::GameScene::Init(bool isDebugMode)
{
	InitDebugMode(isDebugMode);
	ResourceManager.Init();
	partition2D = new Physics2D::SpatialPartition2D();
}

void UEngine::GameScene::InitDebugMode(bool isDebugMode)
{
	this->isDebugMode = isDebugMode;
	if (isDebugMode)
	{
		debugRenderer = new UEngine::DebugRenderer();
		debugRenderer->Init(DXRenderer::Get()->GetDevice(), DXRenderer::Get()->GetImmediateDeviceContext());
	}
}

void UEngine::GameScene::Release()
{
	std::unique_lock<std::mutex> lock(renderMutex);
	renderCondition.wait(lock, [this]() { return renderSyncCount == 0; });

	for (auto obj : gameObjects)
		GameObject::Release(&obj);
	for (auto obj : deleteList)
		GameObject::Release(&obj);
	for (auto obj : creationList)
		GameObject::Release(&obj);
	delete partition2D;
	if (isDebugMode)
	{
		delete debugRenderer;
		debugRenderer = nullptr;
	}
}

void UEngine::GameScene::Update()
{
	while (true)
	{
		partition2D->Release();
		for (auto obj : gameObjects)
			obj->FixedUpdate();
		for (auto obj : gameObjects)
			obj->PhysicsUpdate();

		if (GameState::IsFixedUpdate()) break;
	}
	for (auto obj : gameObjects)
		obj->Update();
	for (auto obj : gameObjects)
		obj->LateUpdate();
	for (auto obj : gameObjects)
		obj->AnimationUpdate();
}

void UEngine::GameScene::Render(ID3D11DeviceContext* deviceContext)
{
	for (auto view : gpu_view)
	{
		WinApplication::Get()->threadPool.AddTask([&, view]()
		{
			std::unique_lock<std::mutex> lock(renderMutex);
			renderSyncCount++;
			lock.unlock();

			auto copeidView = view;
			copeidView.Render(isDebugMode, view.IsMain);

			lock.lock();
			int count = --renderSyncCount;
			if (count == 0) renderCondition.notify_one();
			lock.unlock();
		});
	}

	std::unique_lock<std::mutex> lock(renderMutex);
	renderCondition.wait(lock, [this]() { return renderSyncCount == 0; });

	for (auto view : gpu_view)
		view.PostRender();
}

void UEngine::GameScene::PostRender()
{
	for (auto obj : gameObjects)
		obj->OnPostRender();
}

void UEngine::GameScene::Sync()
{
	for (auto obj : gameObjects)
		obj->Sync();

	gpu_view = cpu_view;
	cpu_view.clear();
	ResourceManager.ApplyChange();

	for (auto obj : gameObjects)
		obj->OnPreRender();

	if (isDebugMode && this == GameState::GetCurrentScene() && MainView) debugRenderer->Flush(MainView->cameraBuffer);
	for (auto obj : gameObjects)
		obj->Initialize();
}

UEngine::GameObject* const UEngine::GameScene::GetGameObject(std::string name)
{
	for (auto obj : gameObjects)
		if (obj->name == name) return obj;
	return nullptr;
}

void UEngine::GameScene::RemoveGameObject(GameObject** obj)
{
	for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
	{
		if (*iter == *obj)
		{
			GameObject::Release(obj);
			gameObjects.erase(iter);
			return;
		}
	}
}

void UEngine::GameScene::RemoveGameObject(std::string name)
{
	for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
	{
		if ((*iter)->name == name)
		{
			gameObjects.erase(iter);
			delete* iter;
			return;
		}
	}
}

void UEngine::GameScene::SaveScene()
{
	XMLSceneParser parser;
	parser.SaveScene(name, isDebugMode, gameObjects);
}

UEngine::GameScene* UEngine::GameScene::LoadScene(std::string name, bool editorMode)
{
	XMLSceneParser parser;
	return parser.LoadScene(name, editorMode);
}