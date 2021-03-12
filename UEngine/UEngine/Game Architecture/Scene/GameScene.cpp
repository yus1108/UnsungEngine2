#include "UEngine.h"
#include "GameScene.h"

void UEngine::GameScene::AddRenderObject(RenderObject* obj)
{
	for (auto iter = renderObjects.begin(); iter != renderObjects.end(); iter++)
		if (*iter == obj) throw std::runtime_error("This object already exsits");
	renderObjects.emplace_back(obj);
}

void UEngine::GameScene::RemoveRenderObject(RenderObject* obj)
{
	for (auto iter = renderObjects.begin(); iter != renderObjects.end(); iter++)
		if (*iter == obj) renderObjects.erase(iter);
	throw std::runtime_error("This object doesn't exsits");
}

void UEngine::GameScene::Load(std::wstring scene_name)
{
	// TODO: Load scene
}

void UEngine::GameScene::Init(bool isDebugMode)
{
	this->isDebugMode = isDebugMode;
	if (isDebugMode)
	{
		debugRenderer = new UEngine::DebugRenderer();
		debugRenderer->Init(DXRenderer::Get()->GetDevice(), DXRenderer::Get()->GetImmediateDeviceContext());
	}
	ResourceManager.Init();
}

void UEngine::GameScene::Release()
{
	for (auto obj : gameObjects)
		GameObject::Release(&obj);
	for (auto obj : deleteList)
		GameObject::Release(&obj);
	for (auto obj : creationList)
		GameObject::Release(&obj);
	for (auto obj : renderObjects)
		delete obj;
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
	if (isDebugMode)
	{
		WinApplication::Get()->threadPool.AddTask([&]()
		{
			std::unique_lock<std::mutex> lock(renderMutex);
			renderSyncCount++;
			lock.unlock();

			if (isDebugMode) debugRenderer->Flush(MainView->cameraBuffer);

			lock.lock();
			int count = --renderSyncCount;
			lock.unlock();

			if (count == 0) renderCondition.notify_one();
		});
	}

	for (auto view : gpu_view)
	{
		WinApplication::Get()->threadPool.AddTask([&]()
		{
			std::unique_lock<std::mutex> lock(renderMutex);
			renderSyncCount++;
			lock.unlock();

			view.Render();

			lock.lock();
			int count = --renderSyncCount;
			lock.unlock();

			if (count == 0) renderCondition.notify_one();
		});
	}
	for (auto obj : gameObjects)
		obj->OnRender();
	

	std::unique_lock<std::mutex> lock(renderMutex);
	renderCondition.wait(lock, [this]() { return renderSyncCount == 0; });

	for (auto view : gpu_view)
		view.Execute(deviceContext);
}

void UEngine::GameScene::PostRender()
{
	for (auto view : gpu_view)
		view.PostRender();
	for (auto obj : gameObjects)
		obj->OnPostRender();
}

void UEngine::GameScene::Sync()
{
	for (auto obj : gameObjects)
		obj->OnPreRender();

	gpu_view = cpu_view;
	cpu_view.clear();

	ResourceManager.ApplyChange();

	for (auto obj : gameObjects)
		obj->Initialize();
}

UEngine::GameObject* const UEngine::GameScene::GetGameObject(std::wstring name)
{
	for (auto obj : gameObjects)
		if (obj->name == name) return obj;
	return nullptr;
}

void UEngine::GameScene::RemoveGameObject(GameObject* obj)
{
	for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
	{
		if (*iter == obj)
		{
			gameObjects.erase(iter);
			return;
		}
	}
}

void UEngine::GameScene::RemoveGameObject(std::wstring name)
{
	for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
	{
		if ((*iter)->name == name)
		{
			gameObjects.erase(iter);
			return;
		}
	}
}
