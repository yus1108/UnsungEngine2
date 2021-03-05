#pragma once

namespace UEngine
{
	class RenderObjectPool
	{
	private:
		ResourceManager* parent_resourceManager;

	private:
		std::map<UINT, std::unordered_map<RenderObject*, RenderObject*>*> pool;
		std::queue<RenderObject*> creationQueue;
		std::map<RenderObject*, RenderObject*> deletionMap;

	public:
		RenderObjectPool(ResourceManager* parent) : parent_resourceManager(parent) {}
		~RenderObjectPool() { Clear(); }
		void Add(RenderObject* const renderObject) { creationQueue.push(renderObject); }
		void Remove(RenderObject* const renderObject) { deletionMap[renderObject] = renderObject; }
		RenderObject* LoadObject(std::string renderMesh, std::string shader);
		void Clear();
		void ClearModels() { DXRenderer::Get()->ResourceManager->RenderObjectPool.Clear(); Clear(); }
		void OnPreRender();
		void OnRender(ID3D11DeviceContext* const deviceContext);
	};
}