#pragma once

#include "ResourceManager\ResourceManager.h"
#include "Game Architecture\Render\GameView.h"
#include "Game Architecture\Physics2D\Collision\SpatialPartition2D.h"

namespace UEngine
{
	class GameState
	{
		friend class Transform;
		friend class Material;
		friend class Camera;
		friend class GameObject;
#pragma region Singleton
	public:
		static GameState* Get() { return &instance; }

	private:
		GameState() = default;
		~GameState() { Release(); }
		static GameState instance;
#pragma endregion
	private:
		bool terminate{ false };

		float currentFixedTimestep{ 0 };
		float fixedUpdateTimer{ 0 };
		float deltatime{ 0 };

		std::list<class GameObject*> gameObjects;
		std::queue<std::pair<GameObject*, std::function<void(GameObject*)>>> loads;


		UEngine::Physics2D::SpatialPartition2D spatialPartition2d;
		DebugRenderer debugRenderer;

	public:
		float FixedTimestep{ 0.02f };
		float MaxFixedTimestep{ 0.1f };

		GameView gameScene;
		ResourceManager ResourceManager{ this };
		Utility::Sync::UThreadPool threadPool;

	private:
		class WinApplication* app{ nullptr };
		class DXRenderer::DXRenderer* renderer{ nullptr };

		void AddObject(GameObject* gameObject) { gameObjects.emplace_back(gameObject); }
	public:
		void Init(WinApplication* app, DXRenderer::DXRenderer* const renderer, size_t numThreads = 8);
		void Release();

		bool GetTerminate() { return terminate; }
		const std::list<class GameObject*>& GetGameObjects() { return gameObjects; }
		UEngine::Physics2D::SpatialPartition2D* const GetSpatialPartition2D() { return &spatialPartition2d; }
		DebugRenderer* const GetDebugRenderer() { return &debugRenderer; }

		void LoadObject(std::function<void(GameObject*)> function);
		void StartGame();

		void Update();
	};
}