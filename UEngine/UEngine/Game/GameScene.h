#pragma once

namespace UEngine
{
	class GameScene
	{
	private:
		RenderObjectPool renderObjectPool;
		DXRenderer::DXView* mainView;
		DXRenderer::DXConstantBuffer* mainCameraBuffer;
		std::queue<DXRenderer::DXView*> viewCreationQueue;
		std::map<DXRenderer::DXView*, DXRenderer::DXView*> view_resources;
		std::map<DXRenderer::DXView*, DXRenderer::DXView*> viewDeletionQueue;
	public:
		virtual ~GameScene();

		// Load DXRenderObject (Instantiate it if not exsist) and return RenderObject
		RenderObject* LoadObject(std::string renderMesh_name, std::string shader_name) { return renderObjectPool.LoadObject(renderMesh_name, shader_name); }
		void AddObject(RenderObject* const renderObject) { renderObjectPool.Add(renderObject); }
		void RemoveObject(RenderObject* const renderObject) { renderObjectPool.Remove(renderObject); }

		void AddView(DXRenderer::DXView* view) { viewCreationQueue.push(view); }
		void RemoveView(DXRenderer::DXView* view) { viewDeletionQueue[view] = view; }

		DXRenderer::DXView* const GetMainView() { return mainView; }

		void OnPreRender();
		void OnRender();
		void OnPostRender();
	};
}
