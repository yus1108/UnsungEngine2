#pragma once

namespace UEngine
{
	class RenderObjectPool
	{
	private:
		std::map<UINT, std::unordered_map<RenderObject*, RenderObject*>*> pool;
		std::queue<RenderObject*> creationQueue;
		std::map<RenderObject*, RenderObject*> deletionMap;
		DXRenderer::DXRenderObjectPool* modelPool;

	public:
		RenderObjectPool() { modelPool = DXRenderer::DXRenderObjectPool::Get(); }
		~RenderObjectPool() { Clear(); }
		void Add(RenderObject* const renderObject) { creationQueue.push(renderObject); }
		void Remove(RenderObject* const renderObject) { deletionMap[renderObject] = renderObject; }
		RenderObject* LoadObject(std::string renderMesh, std::string shader);
		void Clear();
		void ClearModels() { modelPool->Clear(); Clear(); }
		void OnPreRender();
		void OnRender(ID3D11DeviceContext* const deviceContext);
	};
}