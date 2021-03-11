#pragma once
#include "Game Architecture\Object\GameObject.h"

namespace UEngine
{
	struct RenderObject
	{
		std::wstring name;
		UEngine::DXRenderer::DXShader* shader{ nullptr };
		UEngine::DXRenderer::DXRenderMesh* renderMesh{ nullptr };
		std::map<std::string, UEngine::DXRenderer::DXConstantBuffer*> constantBuffers;
		std::map<std::string, UEngine::DXRenderer::DXTexture*> textures;
	};

	class GameView
	{
	public:
		bool isRenderable{ true };
		UEngine::DXRenderer::DXView* view{ nullptr };
		std::vector<RenderObject*> renderObjects;
	};

	class GameScene
	{
		std::list<class GameObject*> gameObjects;
		std::map<std::wstring, RenderObject*> renderObjects;
		std::vector<GameView*> gpu_view;

	public:
		std::vector<GameView*> cpu_view;
		UEngine::DXRenderer::DXResourceManager ResourceManager;
		DebugRenderer DebugRenderer;
		// TODO: Add collisions

	public:
		template <typename T>
		T* LoadResource(std::wstring resource_name);

		virtual void Load() {}
		void Load(std::wstring scene_name) {}

		void Update() {}
		void Render() {}
		void Sync() {}

		RenderObject* GetRenderObject(std::wstring name)
		{
			auto obj = renderObjects.find(name);
			if (obj == renderObjects.end()) return nullptr;
			return obj->second;
		}
		void AddRenderObject(RenderObject* obj) 
		{ 
			if (GetRenderObject(obj->name) != nullptr)
				throw std::runtime_error("This object already exsits");
			renderObjects[obj->name] = obj;
		}
		void RemoveRenderObject(RenderObject* obj)
		{
			if (GetRenderObject(obj->name) == nullptr)
				throw std::runtime_error("This object doesn't exsits");
			renderObjects.erase(obj->name);
			delete obj;
		}

		void AddGameObject(GameObject* obj) { gameObjects.emplace_back(obj); }
		GameObject* GetGameObject(std::wstring name)
		{
			for (auto obj : gameObjects)
				if (obj->name == name) return obj;
			return nullptr;
		}

		void RemoveGameObject(GameObject* obj)
		{
			for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
			{
				if (*iter == obj)
				{
					gameObjects.erase(iter);
					return;
				}
			}
		}
		void RemoveGameObject(std::wstring name)
		{
			for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
			{
				if ((*iter)->name == name)
				{
					gameObjects.erase(iter);
					return;
				}
			}
		}
	};

	// TODO: Make singleton
	class GameState
	{
	private:
		bool isTerminate{ false };
		std::map<std::wstring, GameScene*> scenes;
		GameScene* currentScene = nullptr;

#pragma region Singleton
	public:
		static GameState* Get() { return &instance; }
		static GameScene* GetCurrentScene() { return instance.currentScene; }
		static bool GetTerminate() { return instance.isTerminate; }

	private:
		GameState() = default;
		~GameState() { Release(); }
		static GameState instance;
		void Release();
#pragma endregion
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
