#include "UEngine.h"
#include "RenderObjectPool.h"

UEngine::RenderObject* UEngine::RenderObjectPool::LoadObject(std::string renderMesh, std::string shader)
{
	auto DXRObject = DXRenderer::Get()->ResourceManager->RenderObjectPool.LoadObject(renderMesh, shader);
	RenderObject* renderObject = new RenderObject();
	renderObject->objectNumber = DXRObject->GetID();
	return renderObject;
}

void UEngine::RenderObjectPool::Clear()
{
	while (!creationQueue.empty())
	{
		delete creationQueue.front();
		creationQueue.pop();
	}
	for (auto objectPair : deletionMap)
	{
		if (objectPair.second == nullptr) continue;
		auto object = objectPair.second;
		pool[object->objectNumber]->erase(object);
		if (pool[object->objectNumber]->size() == 0)
		{
			delete pool[object->objectNumber];
			pool.erase(object->objectNumber);
		}
		delete object;
	}
	for (auto list : pool)
	{
		for (auto object : *list.second)
			delete object.second;
		delete list.second;
	}
	deletionMap.clear();
	pool.clear();
	orderPool.clear();
}

void UEngine::RenderObjectPool::OnPreRender()
{
	DXRenderer::Get()->ResourceManager->RenderObjectPool.OnPreRender();
	while (!creationQueue.empty())
	{
		auto curr = creationQueue.front();
		creationQueue.pop();
		if (curr == nullptr) continue;
		if (pool[curr->objectNumber] == nullptr) pool[curr->objectNumber] = new std::unordered_map<RenderObject*, RenderObject*>();
		(*pool[curr->objectNumber])[curr] = curr;
		orderPool.emplace_back(curr);
	}
	for (auto objectPair : deletionMap)
	{
		auto object = objectPair.second;
		pool[object->objectNumber]->erase(object);
		std::vector<std::vector<RenderObject*>::iterator> iters;
		for (auto iter = orderPool.begin(); iter != orderPool.end(); iter++)
		{
			if (*iter == object)
			{
				iters.push_back(iter);
			}
		}
		for (size_t i = 0; i < iters.size(); i++)
		{
			for (auto iter = orderPool.begin(); iter != orderPool.end(); iter++)
			{
				if (*iter == object)
				{
					orderPool.erase(iter);
					break;
				}
			}
		}
		if (pool[object->objectNumber]->size() == 0)
		{
			delete pool[object->objectNumber];
			pool.erase(object->objectNumber);
		}
		delete object;
	}
	deletionMap.clear();
}

void UEngine::RenderObjectPool::OnRender(ID3D11DeviceContext* deviceContext)
{
	// render
	for (auto object : orderPool)
	{
		auto model = DXRenderer::Get()->ResourceManager->RenderObjectPool.GetObjectByID(object->objectNumber);
		model->Set(deviceContext);
		for (auto bufferPair : object->constantBuffers)
			bufferPair.second->Set(deviceContext);
		if (object->imageTexture != nullptr)
			object->imageTexture->Set(deviceContext);
		model->Draw(deviceContext);
	}
}
