#pragma once

namespace UEngine
{
	class RenderObjectPool
	{
	private:
		std::map<UINT, std::unordered_map<RenderObject*, RenderObject*>*> pool;
		std::queue<RenderObject*> creationQueue;
		std::map<RenderObject*, RenderObject*> deletionMap;

	public:
		~RenderObjectPool();
		void Add(RenderObject* const renderObject) { creationQueue.push(renderObject); }
		void Remove(RenderObject* const renderObject) { deletionMap[renderObject] = renderObject; }
		void OnPreRender();
		void OnRender(ID3D11DeviceContext* const deviceContext, const std::vector<DXRenderer::DXRenderObject*>* const resources);
	};
}