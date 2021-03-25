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

		std::string name;

		class Camera* MainCamera{ nullptr };
		GameView* MainView{ nullptr };

		std::vector<GameView> cpu_view;

		UEngine::DXRenderer::DXResourceManager ResourceManager;
		DebugRenderer *debugRenderer = nullptr;
		Physics2D::SpatialPartition2D *partition2D = nullptr;

		template <typename T>
		T* LoadResource(std::wstring resource_name);

		void Init(bool isDebugMode);
		void InitDebugMode(bool isDebugMode);
		void Release();

		void Update();
		void Render(ID3D11DeviceContext* deviceContext);
		void PostRender();
		void Sync();

		bool IsDebugMode() { return isDebugMode; }
		GameObject* const GetGameObject(std::string name);
		const std::list<class GameObject*> GetGameObjects() { return gameObjects; }
		const std::vector<GameView>& GetGpuViews() { return gpu_view; }

		void AddGameObject(GameObject* obj) { gameObjects.emplace_back(obj); }
		void RemoveGameObject(GameObject** obj);
		void RemoveGameObject(std::string name);

		void SaveScene();
		static UEngine::GameScene* LoadScene(std::string name, bool editorMode);
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