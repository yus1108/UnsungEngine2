#pragma once

namespace UEngine
{
	class GameScene
	{
	private:
		RenderObjectPool renderObjectPool;
		DXRenderer::DXView* mainView;
		DXRenderer::DXView* nextMainView;
		std::queue<DXRenderer::DXView*> viewCreationQueue;
		std::list<DXRenderer::DXView*> view_resources;
		std::map<DXRenderer::DXView*, DXRenderer::DXView*> viewDeletionQueue;
	public:

		void AddObject(RenderObject* const renderObject) { renderObjectPool.Add(renderObject); }
		void RemoveObject(RenderObject* const renderObject) { renderObjectPool.Remove(renderObject); }

		void SetView(DXRenderer::DXView* nextMainView) { this->nextMainView = nextMainView; }
		void AddView(DXRenderer::DXView* view) { viewCreationQueue.push(view); }
		void RemoveView(DXRenderer::DXView* view) { viewDeletionQueue[view] = view; }

		DXRenderer::DXView* const GetMainView() { return mainView; }

		void OnPreRender();
		void OnRender();
		void OnPostRender();
	};
}
