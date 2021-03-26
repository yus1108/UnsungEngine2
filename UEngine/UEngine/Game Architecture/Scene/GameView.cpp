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
		for (size_t i = 0; i < renderObject->constantBuffers.size(); i++)
			renderObject->constantBuffers[i]->Set(view->GetDeviceContext());
		if (renderObject->imageTextures) renderObject->imageTextures->Set(view->GetDeviceContext(), 0);
		if (renderObject->imageSamplerState) renderObject->imageSamplerState->Set(view->GetDeviceContext(), 0);
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
