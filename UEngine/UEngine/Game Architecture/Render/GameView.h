#pragma once
#include "Game Architecture\GameState.h"

namespace UEngine
{
	class GameView
	{
	private:
		DXRenderer::DXView* mainView{ nullptr };
		DXRenderer::DXConstantBuffer* mainCameraBuffer{ nullptr };
		std::queue<DXRenderer::DXView*> viewCreationQueue;
		std::map<DXRenderer::DXView*, DXRenderer::DXView*> view_resources;
		std::map<DXRenderer::DXView*, DXRenderer::DXView*> viewDeletionQueue;
	public:
		virtual ~GameView();

		// Load DXRenderObject (Instantiate it if not exsist) and return RenderObject
		RenderObject* LoadObject(std::string renderMesh_name, std::string shader_name);
		void AddObject(RenderObject* const renderObject);
		void RemoveObject(RenderObject* const renderObject);

		void AddView(DXRenderer::DXView* view) { viewCreationQueue.push(view); }
		void RemoveView(DXRenderer::DXView* view) { viewDeletionQueue[view] = view; }

		DXRenderer::DXView* const GetMainView() { return mainView; }
		DXRenderer::DXConstantBuffer* const GetMainViewCBuffer() { return mainCameraBuffer; }

		void OnPreRender();
		void OnRender();
		void OnPostRender();
	};
}