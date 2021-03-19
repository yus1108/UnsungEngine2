#include "UEngine.h"
#include "GameView.h"

void UEngine::GameView::Render(bool isDebugMode, bool isMainView)
{
	if (!isRenderable) return;
	view->Begin();
	cameraBuffer->Set(view->GetDeviceContext());
	for (auto renderObject : renderObjects)
	{
		if (!renderObject->isRenderable)
			continue;
		for (auto cbufferMap : renderObject->constantBuffers)
			cbufferMap.second->Set(view->GetDeviceContext());
		for (auto textureMap : renderObject->textures)
			textureMap.second->Set(view->GetDeviceContext(), textureMap.first);
		for (auto samplerMap : renderObject->samplerState)
			samplerMap.second->Set(view->GetDeviceContext(), samplerMap.first);
		renderObject->shader->Set(view->GetDeviceContext());
		renderObject->renderMesh->Set(view->GetDeviceContext());
		renderObject->renderMesh->Draw(view->GetDeviceContext());
	}

	if (isDebugMode && isMainView)
	{
		DXRenderer::Get()->Begin(view->GetContext(), false);
		GameState::GetCurrentScene()->debugRenderer->Render(view->GetDeviceContext());
		DXRenderer::Get()->Draw(view->GetContext(), GameState::GetCurrentScene()->debugRenderer->GetAddressOfViewResource());
		DXRenderer::Get()->End(view);
	}

	view->End();
}

void UEngine::GameView::PostRender()
{
	if (!isRenderable) return;
	view->Execute(DXRenderer::Get()->GetImmediateDeviceContext());
}
