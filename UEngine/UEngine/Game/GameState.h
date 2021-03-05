#pragma once

#include "ResourceManager\ResourceManager.h"
#include "Game\Render\GameView.h"
#include "Game Architecture\Collision\SpatialPartition2D.h"

namespace UEngine
{
	class GameState
	{
		friend class Transform;
		friend class Material;
		friend class Camera;
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


		SpatialPartition2D spatialPartition2d;
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

	public:
		void Init(WinApplication* app, DXRenderer::DXRenderer* const renderer, size_t numThreads = 8);
		void Release();

		bool GetTerminate() { return terminate; }
		const std::list<class GameObject*>& GetGameObjects() { return gameObjects; }
		SpatialPartition2D* const GetSpatialPartition2D() { return &spatialPartition2d; }
		DebugRenderer* const GetDebugRenderer() { return &debugRenderer; }

		void AddObject(GameObject* gameObject) { gameObjects.emplace_back(gameObject); }

		void Update();
	};
}