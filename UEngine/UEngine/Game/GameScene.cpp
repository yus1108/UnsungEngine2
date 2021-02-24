#include "UEngine.h"
#include "GameScene.h"

void UEngine::GameScene::OnPreRender()
{
	renderObjectPool.OnPreRender();
}

void UEngine::GameScene::OnRender()
{
	mainView->Begin();
	renderObjectPool.OnRender(mainView->GetDeviceContext(), DXRenderer::DXRenderObjectPool::Get()->GetList());
	mainView->End();
}

void UEngine::GameScene::OnPostRender()
{
	mainView->Execute(DXRenderer::Get()->GetImmediateDeviceContext());
}
