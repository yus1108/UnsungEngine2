#pragma once
#include "DXResourceManager.h"

namespace UEngine
{
	namespace DXRenderer
	{
		namespace ResourceManager
		{
			class DXSceneManager final
			{
			private:
				class DXResourceManager* parent_dxResourceManager;

			private:
				std::map<std::wstring, class UEngine::DXRenderer::DXScene*> resources;
				std::map<std::wstring, DXScene*> creationQueue;
				std::queue<DXScene*> deletionQueue;

			public:
				DXSceneManager() = delete;
				DXSceneManager(const DXScene&) = delete;
				DXSceneManager(DXResourceManager* parent) : parent_dxResourceManager(parent) {}
				~DXSceneManager() { Release(); }

				void LoadScene(std::wstring name);
				const DXScene* const GetScene(std::wstring name) { return resources[name]; }
				void UnLoadScene(std::wstring name);

				void Update();

				void Begin(std::wstring name);
				void Render(std::wstring name);
				void End(std::wstring name);

				void Release();
			};
		}
	}
}
