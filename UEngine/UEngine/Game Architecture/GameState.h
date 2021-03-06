#pragma once
#include "Game Architecture\Object\GameObject.h"

namespace UEngine
{
	class GameState
	{
	private:
		bool isTerminate{ false };
		bool drawOnBackBuffer{ true };

		float deltaTime = 0.0f;
		float fixedUpdateTimer = 0.0f;

		std::map<std::string, GameScene*> scenes;
		GameScene* currentScene = nullptr;

		void RenderHierarchy(UEngine::GameObject** gameObject);
		void HierarchyPopup(GameScene* scene, GameObject** gameObject);
#pragma region Singleton
	private:
		GameState() = default;
		~GameState() = default;
		static GameState* instance;
#pragma endregion

	public:
		bool isFocused{ true };
		bool noUpdate{ false };
		Utility::Sync::UThreadSync stateSync;
		Vector2 startWindowPos{ 0, 0 };
		Vector2 windowSize{ 0, 0 };

		static void Attach(GameState* state) { instance = state; }
		static void Detach() { instance = nullptr; }

		float FixedTimestep = 0.02f;
		float MaxFixedTimestep = 0.1f;

		static GameState* Get() { if (instance == nullptr) instance = new GameState; return instance; }
		static GameScene* GetCurrentScene() { return instance->currentScene; }
		static void SetCurrentScene(std::string name);

		static void AddScene(GameScene* scene, bool setCurrentScene = false);
		static GameScene* GetScene(std::string name);
		static const std::map<std::string, GameScene*> GetScenes() { return instance->scenes; }

		static bool IsTerminate() { return instance->isTerminate; }
		static bool IsFixedUpdate();

		static void Terminate() { instance->isTerminate = true; }

		static float GetCurrentFixedTimestep() { return UEngine::Math::Clamp(instance->deltaTime, instance->FixedTimestep, instance->MaxFixedTimestep); }
		static float GetDeltaTime() { return instance->deltaTime; }

		static void Init(bool drawOnBackBuffer = true);
		static void Init(GameScene* scene, bool drawOnBackBuffer = true);
		static void Update(std::function<void()> OnSync, std::function<bool()> OnUpdate, std::function<void()> OnRender);
		static void ShowHierarchy();
		static void Release();
	};
}
