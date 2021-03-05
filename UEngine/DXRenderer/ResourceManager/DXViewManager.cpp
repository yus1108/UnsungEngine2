#include "dxrframework.h"
#include "DXViewManager.h"

void UEngine::DXRenderer::ResourceManager::DXViewManager::Add(DXView* view)
{
	creationQueue[view] = view;
}

void UEngine::DXRenderer::ResourceManager::DXViewManager::Remove(DXView* view)
{
	auto resource = resources.find(view);
	auto creation = creationQueue.find(view);
	if (resource == resources.end() && creation == creationQueue.end())
		throw std::runtime_error("Caanot find the scene with the given name");

	if (resource != resources.end())
	{
		deletionQueue.push(resource->second);
		return;
	}

	DXView::Release(&creation->second);
	creationQueue.erase(view);
}

void UEngine::DXRenderer::ResourceManager::DXViewManager::Update()
{
	for (auto resource : creationQueue)
		resources[resource.first] = resource.second;
	creationQueue.clear();
	while (!deletionQueue.empty())
	{
		auto toDelete = deletionQueue.front();
		resources.erase(toDelete);
		DXView::Release(&toDelete);
		deletionQueue.pop();
	}
}

void UEngine::DXRenderer::ResourceManager::DXViewManager::Release()
{
	for (auto resource : creationQueue)
		DXView::Release(&resource.second);
	creationQueue.clear();
	while (!deletionQueue.empty())
	{
		DXView::Release(&deletionQueue.front());
		deletionQueue.pop();
	}
	for (auto resource : resources)
		DXView::Release(&resource.second);
	resources.clear();
}
