#pragma once

namespace UEngine
{
	class GameView
	{
	public:
		bool isRenderable{ true };
		bool IsMain{ false };
		UEngine::DXRenderer::DXView* view{ nullptr };
		UEngine::DXRenderer::DXConstantBuffer* cameraBuffer{ nullptr };
		std::vector<RenderObject*> renderObjects;

		void Render(bool isDebugMode, bool isMainView);
		void PostRender();
	};
}