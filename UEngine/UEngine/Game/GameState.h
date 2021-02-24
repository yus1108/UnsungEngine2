#pragma once

namespace UEngine
{
	class GameState
	{
#pragma region Singleton
	public:
		static GameState* Get() { return &instance; }

	private:
		GameState() = default;
		~GameState() { Release(); }
		static GameState instance;
#pragma endregion
	private:
		Utility::Sync::UThreadPool threadPool;
		std::list<class GameObject*> gameObjects;

	public:
		GameScene gameScene;

	private:
		class WinApplication* app{ nullptr };
		class DXRenderer::DXRenderer* renderer{ nullptr };

	public:
		void Init(WinApplication* app, DXRenderer::DXRenderer* const renderer, size_t numThreads = 8);
		void Release();

		void AddObject(GameObject* gameObject) { gameObjects.push_back(gameObject); }

		void Update();
	};
}