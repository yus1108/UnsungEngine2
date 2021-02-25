#include "UEngine.h"
#include "RenderObjectPool.h"

UEngine::RenderObject* UEngine::RenderObjectPool::LoadObject(std::string renderMesh, std::string shader)
{
	auto DXRObject = modelPool->LoadObject(renderMesh, shader);
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
}

void UEngine::RenderObjectPool::OnPreRender()
{
	modelPool->OnPreRender();
	while (!creationQueue.empty())
	{
		auto curr = creationQueue.front();
		creationQueue.pop();
		if (curr == nullptr) continue;
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

void UEngine::RenderObjectPool::OnRender(ID3D11DeviceContext* deviceContext)
{
	// render
	for (auto desc : pool)
	{
		auto model = modelPool->GetObjectByID(desc.first);
		model->Set(deviceContext);
		for (auto objectPair : *desc.second)
		{
			for (auto bufferPair : objectPair.second->constantBuffers)
				bufferPair.second->Set(deviceContext);
			model->Draw(deviceContext);
		}
	}
}
