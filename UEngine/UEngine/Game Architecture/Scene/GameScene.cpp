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
	for (auto obj : gameObjects)
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
		auto fRender = sceneSync.CreateSyncTask([view, this]()
		{
			static_cast<GameView>(view).Render(isDebugMode, view.IsMain);
		});
		WinApplication::Get()->threadPool.AddTask(fRender);
	}
	sceneSync.Join();

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
	if (isDebugMode && this == GameState::GetCurrentScene() && MainView) debugRenderer->Flush(MainView->cameraBuffer);

	for (auto obj : creationList)
		gameObjects.emplace_back(obj);
	for (auto diter = deletionList.begin(); diter != deletionList.end(); diter++)
	{
		for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
		{
			if (*iter == *diter)
			{
				GameObject::Release(&(*iter));
				gameObjects.erase(iter);
				break;
			}
		}
	}
	deletionList.clear();
	creationList.clear();

	ResourceManager.ApplyChange();

	for (auto obj : gameObjects)
		obj->Initialize();

	for (auto obj : gameObjects)
		obj->Sync();

	gpu_view.clear();
	for (auto obj : gameObjects)
		obj->OnPreRender();
}

UEngine::GameObject* const UEngine::GameScene::GetGameObject(std::string name)
{
	for (auto obj : gameObjects)
		if (obj->name == name) return obj;
	return nullptr;
}

void UEngine::GameScene::RemoveGameObject(GameObject** obj)
{
	deletionList.emplace_back(*obj);
	*obj = nullptr;
}

void UEngine::GameScene::RemoveGameObject(std::string name)
{
	for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
	{
		if ((*iter)->name == name)
		{
			deletionList.emplace_back(*iter);
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