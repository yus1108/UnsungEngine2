#pragma once
#include "Game Architecture\Physics2D\SpatialPartition2D.h"

namespace UEngine
{
	class GameScene
	{
		friend class RenderComponent;
	private:
		bool isDebugMode = false;

		int renderSyncCount = 0;
		std::mutex renderMutex;
		std::condition_variable renderCondition;

		std::list<class GameObject*> gameObjects, deleteList, creationList;
		std::vector<GameView> gpu_view;

	public:
		~GameScene() { Release(); }

		std::wstring name;

		class Camera* MainCamera;
		GameView* MainView;
		std::vector<GameView> cpu_view;

		UEngine::DXRenderer::DXResourceManager ResourceManager;
		DebugRenderer *debugRenderer = nullptr;
		Physics2D::SpatialPartition2D partition2D;

		template <typename T>
		T* LoadResource(std::wstring resource_name);

		void Init(bool isDebugMode);
		void Release();

		void Load(std::wstring scene_name);

		void Update();
		void Render(ID3D11DeviceContext* deviceContext);
		void PostRender();
		void Sync();

		bool IsDebugMode() { return isDebugMode; }
		GameObject* const GetGameObject(std::wstring name);
		const std::vector<GameView>& GetGpuViews() { return gpu_view; }

		void AddGameObject(GameObject* obj) { gameObjects.emplace_back(obj); }
		void RemoveGameObject(GameObject* obj);
		void RemoveGameObject(std::wstring name);
	};

	template<typename T>
	inline T* GameScene::LoadResource(std::wstring resource_name)
	{
		auto resource = ResourceManager->GetResource<T>(resource_name);
		if (resource == nullptr)
		{
			// TODO: open file and load resource using reousrce_name as file name
		}
		return resource;
	}
}