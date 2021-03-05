#pragma once
#include "DXResourceManager.h"

namespace UEngine
{
	namespace DXRenderer
	{
		class DXRenderObjectPool
		{
		private:
			class DXResourceManager* parent_dxResourceManager;

		private:
			std::map<std::string, DXRenderObject*> poolMap;
			std::map<UINT, DXRenderObject*> poolVector;
			std::queue<DXRenderObject*> creationQueue;
			std::queue<DXRenderObject*> deletionQueue;

		public:
			DXRenderObjectPool(DXResourceManager* parent) : parent_dxResourceManager(parent) {}
			~DXRenderObjectPool() { Clear(); }

			DXRenderObject* LoadObject(std::string renderMesh, std::string shader);
			void Clear();

			void Add(DXRenderObject* const renderObject) { creationQueue.push(renderObject); }
			void Remove(DXRenderObject* renderObject) { deletionQueue.push(renderObject); }

			UEngine::DXRenderer::DXRenderObject* GetObjectByID(UINT id) { return poolVector[id]; }
			UEngine::DXRenderer::DXRenderObject* GetObjectByName(std::string name) { return poolMap[name]; }

			void OnPreRender();
		};
	}
}



