#include "UEngine.h"
#include "RenderObjectPool.h"

UEngine::RenderObjectPool::~RenderObjectPool()
{
	while (!creationQueue.empty())
	{
		delete creationQueue.front();
		creationQueue.pop();
	}
	for (auto objectPair : deletionMap)
	{
		auto object = objectPair.second;
		delete (*pool[object->objectNumber])[object];
		pool[object->objectNumber]->erase(object);
		if (pool[object->objectNumber]->size() == 0)
		{
			delete pool[object->objectNumber];
			pool.erase(object->objectNumber);
		}
	}
	for (auto list : pool)
	{
		for (auto object : *list.second)
			delete object.second;
		delete list.second;
	}
}

void UEngine::RenderObjectPool::OnPreRender()
{
	while (!creationQueue.empty())
	{
		auto curr = creationQueue.front();
		creationQueue.pop();
		if (pool[curr->objectNumber] == nullptr) pool[curr->objectNumber] = new std::unordered_map<RenderObject*, RenderObject*>();
		(*pool[curr->objectNumber])[curr] = curr;
	}
	for (auto objectPair : deletionMap)
	{
		auto object = objectPair.second;
		delete (*pool[object->objectNumber])[object];
		pool[object->objectNumber]->erase(object);
		if (pool[object->objectNumber]->size() == 0)
		{
			delete pool[object->objectNumber];
			pool.erase(object->objectNumber);
		}
	}
	deletionMap.clear();
}

void UEngine::RenderObjectPool::OnRender(ID3D11DeviceContext* deviceContext, const std::vector<DXRenderer::DXRenderObject*>* const resources)
{
	// render
	for (auto desc : pool)
	{
		auto model = (*resources)[desc.first];
		model->Set(deviceContext);
		for (auto objectPair : *desc.second)
		{
			for (auto bufferPair : objectPair.second->constantBuffers)
				bufferPair.second->Set(deviceContext);
			model->Draw(deviceContext);
		}
	}
}
