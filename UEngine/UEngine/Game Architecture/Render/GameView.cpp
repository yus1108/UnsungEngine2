#include "UEngine.h"
#include "GameView.h"

UEngine::GameView::~GameView()
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

UEngine::RenderObject* UEngine::GameView::LoadObject(std::string renderMesh_name, std::string shader_name)
{ 
	return GameState::Get()->ResourceManager.renderObjectPool.LoadObject(renderMesh_name, shader_name);
}

void UEngine::GameView::AddObject(RenderObject* const renderObject)
{ 
	GameState::Get()->ResourceManager.renderObjectPool.Add(renderObject);
}

void UEngine::GameView::RemoveObject(RenderObject* const renderObject)
{ 
	GameState::Get()->ResourceManager.renderObjectPool.Remove(renderObject);
}

void UEngine::GameView::OnPreRender()
{
	GameState::Get()->ResourceManager.renderObjectPool.OnPreRender();
	if (mainView == nullptr)
	{
		mainView = Camera::mainCamera->view;
		mainCameraBuffer = Camera::mainCamera->cameraBuffer;
	}
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

void UEngine::GameView::OnRender()
{
	if (!mainView) throw std::out_of_range("there is no main view");
	mainView->Begin();
	mainCameraBuffer->Set(mainView->GetDeviceContext());
	GameState::Get()->ResourceManager.renderObjectPool.OnRender(mainView->GetDeviceContext());
	mainView->End();
}

void UEngine::GameView::OnPostRender()
{
	mainView->Execute(DXRenderer::Get()->GetImmediateDeviceContext());
}
