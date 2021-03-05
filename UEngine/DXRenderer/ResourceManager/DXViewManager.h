#pragma once
#include "DXResourceManager.h"

namespace UEngine
{
	namespace DXRenderer
	{
		namespace ResourceManager
		{
			class DXViewManager final
			{
			private:
				class DXResourceManager* parent_dxResourceManager;

			private:
				std::map<class UEngine::DXRenderer::DXView*, DXView*> resources;
				std::map<DXView*, DXView*> creationQueue;
				std::queue<DXView*> deletionQueue;

			public:
				DXViewManager() = delete;
				DXViewManager(const DXViewManager&) = delete;
				DXViewManager(DXResourceManager* parent) : parent_dxResourceManager(parent) {}
				~DXViewManager() { Release(); }

				void Add(DXView* view);
				void Remove(DXView* view);

				void Update();

				void Release();
			};
		}
	}
}