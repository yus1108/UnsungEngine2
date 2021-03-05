#include "dxrframework.h"
#include "DXSceneManager.h"

void UEngine::DXRenderer::ResourceManager::DXSceneManager::LoadScene(std::wstring name)
{
	auto scene = new DXScene(name);
	creationQueue[name] = scene;
}

void UEngine::DXRenderer::ResourceManager::DXSceneManager::UnLoadScene(std::wstring name)
{
	auto resource = resources.find(name);
	auto creation = creationQueue.find(name);
	if (resource == resources.end() && creation == creationQueue.end())
		throw std::runtime_error("Caanot find the scene with the given name");

	if (resource != resources.end())
	{
		deletionQueue.push(resource->second);
		return;
	}

	delete creation->second;
	creationQueue.erase(name);
}

void UEngine::DXRenderer::ResourceManager::DXSceneManager::Update()
{
	for (auto resource : creationQueue)
		resources[resource.first] = resource.second;
	creationQueue.clear();
	while (!deletionQueue.empty())
	{
		auto toDelete = deletionQueue.front();
		resources.erase(toDelete->GetName());
		delete toDelete;
		deletionQueue.pop();
	}
}

void UEngine::DXRenderer::ResourceManager::DXSceneManager::Begin(std::wstring name)
{
	auto resource = resources.find(name);
	if (resource == resources.end()) throw std::runtime_error("Cannot find the scene");
	resource->second->Begin();
}

void UEngine::DXRenderer::ResourceManager::DXSceneManager::Render(std::wstring name)
{
	auto resource = resources.find(name);
	if (resource == resources.end()) throw std::runtime_error("Cannot find the scene");
	resource->second->Render();
}

void UEngine::DXRenderer::ResourceManager::DXSceneManager::End(std::wstring name)
{
	auto resource = resources.find(name);
	if (resource == resources.end()) throw std::runtime_error("Cannot find the scene");
	resource->second->End();
}

void UEngine::DXRenderer::ResourceManager::DXSceneManager::Release()
{
	for (auto resource : creationQueue)
		delete resource.second;
	creationQueue.clear();
	while (!deletionQueue.empty())
	{
		delete deletionQueue.front();
		deletionQueue.pop();
	}
	for (auto resource : resources)
		delete resource.second;
	resources.clear();
}
