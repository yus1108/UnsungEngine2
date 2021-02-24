#include "UEngine.h"
#include "GameScene.h"

UEngine::GameScene::~GameScene()
{
	while (!viewCreationQueue.empty())
	{
		DXRenderer::DXView::Release(&viewCreationQueue.front());
		viewCreationQueue.pop();
	}
	for (auto objectPair : viewDeletionQueue)
	{
		auto objectToDelete = objectPair.second;
		view_resources.erase(objectToDelete);
		DXRenderer::DXView::Release(&objectToDelete);
	}
	for (auto object : view_resources)
	{
		DXRenderer::DXView::Release(&object.second);
	}
}

void UEngine::GameScene::OnPreRender()
{
	renderObjectPool.OnPreRender();
	mainView = nextMainView;
	while (!viewCreationQueue.empty())
	{
		auto curr = viewCreationQueue.front();
		viewCreationQueue.pop();
		view_resources[curr] = curr;
	}
	for (auto objectPair : viewDeletionQueue)
	{
		auto objectToDelete = objectPair.second;
		view_resources.erase(objectToDelete);
		DXRenderer::DXView::Release(&objectToDelete);
	}
	viewDeletionQueue.clear();
}

void UEngine::GameScene::OnRender()
{
	if (!mainView) throw std::out_of_range("there is no main view");
	mainView->Begin();
	renderObjectPool.OnRender(mainView->GetDeviceContext(), DXRenderer::DXRenderObjectPool::Get()->GetList());
	mainView->End();
}

void UEngine::GameScene::OnPostRender()
{
	mainView->Execute(DXRenderer::Get()->GetImmediateDeviceContext());
}
