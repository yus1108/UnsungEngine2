#pragma once

namespace UEngine
{
	class GameView
	{
	public:
		bool isRenderable{ true };
		UEngine::DXRenderer::DXView* view{ nullptr };
		UEngine::DXRenderer::DXConstantBuffer* cameraBuffer{ nullptr };
		std::vector<RenderObject*> renderObjects;

		void Render();
		void PostRender();
		void Execute(ID3D11DeviceContext* deviceContext) { view->Execute(deviceContext); }
	};
}