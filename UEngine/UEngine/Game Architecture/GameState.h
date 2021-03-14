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

		std::map<std::wstring, GameScene*> scenes;
		GameScene* currentScene = nullptr;

#pragma region Singleton
	private:
		GameState() = default;
		~GameState() = default;
		static GameState *instance;
#pragma endregion

	public:
		float FixedTimestep = 0.02f;
		float MaxFixedTimestep = 0.1f;

		static GameState* Get() { return instance; }
		static GameScene* GetCurrentScene() { return instance->currentScene; }

		static bool IsTerminate() { return instance->isTerminate; }
		static bool IsFixedUpdate();

		static float GetCurrentFixedTimestep() { return UEngine::Math::Clamp(instance->deltaTime, instance->FixedTimestep, instance->MaxFixedTimestep); }
		static float GetDeltaTime() { return instance->deltaTime; }

		static void Init(GameScene* scene, bool drawOnBackBuffer = true);
		static void Update(std::function<bool()> OnUpdate, std::function<void()> OnRender);
		static void Release();
	};
}
