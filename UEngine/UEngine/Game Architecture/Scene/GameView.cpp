#include "UEngine.h"
#include "GameView.h"

void UEngine::GameView::Render()
{
	if (!isRenderable) return;
	view->Begin();
	cameraBuffer->Set(view->GetDeviceContext());
	for (auto renderObject : renderObjects)
	{
		for (auto cbufferMap : renderObject.constantBuffers)
			cbufferMap.second->Set(view->GetDeviceContext());
		for (auto textureMap : renderObject.textures)
			textureMap.second->Set(view->GetDeviceContext());
		renderObject.shader->Set(view->GetDeviceContext());
		renderObject.renderMesh->Set(view->GetDeviceContext());
		renderObject.renderMesh->Draw(view->GetDeviceContext());
	}
	view->End();
}

void UEngine::GameView::PostRender()
{
	if (!isRenderable) return;
	view->Execute(DXRenderer::Get()->GetImmediateDeviceContext());
}
