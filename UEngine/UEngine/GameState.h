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
		std::unordered_map<std::string, GameObject*> gameObjects;

	public:
		class WinApplication* app{ nullptr };
		class DXRenderer::DXRenderer* renderer{ nullptr };
		std::vector<class Camera*> cameras;

	public:
		void Init(WinApplication* app, DXRenderer::DXRenderer* const renderer, size_t numThreads = 8);
		void Release();

		void AddGameObject(std::string name, GameObject* gameObject);
		void LoadScene(std::string file_name, std::function<void()> loadingScene = nullptr);

		void Update();
	};
}