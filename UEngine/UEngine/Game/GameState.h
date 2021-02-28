#pragma once

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
		Utility::Sync::UThreadPool threadPool;
		std::list<class GameObject*> gameObjects;
		ConstantBufferPool constantBufferPool;

	public:
		GameScene gameScene;
		DebugRenderer debugRenderer;

	private:
		class WinApplication* app{ nullptr };
		class DXRenderer::DXRenderer* renderer{ nullptr };

	public:
		void Init(WinApplication* app, DXRenderer::DXRenderer* const renderer, size_t numThreads = 8);
		void Release();

		bool GetTerminate() { return terminate; }

		void AddObject(GameObject* gameObject) { gameObjects.push_back(gameObject); }

		void Update();
	};
}