#include "dxrframework.h"
#include "DXRenderObjectPool.h"

UEngine::DXRenderer::DXRenderObjectPool UEngine::DXRenderer::DXRenderObjectPool::instance;

UEngine::DXRenderer::DXRenderObject* UEngine::DXRenderer::DXRenderObjectPool::LoadObject(std::string renderMesh, std::string shader)
{
	if (poolMap[renderMesh + shader] != nullptr)
		return poolMap[renderMesh + shader];

	auto object = DXRenderObject::Instantiate(renderMesh, shader);
	creationQueue.push(object);
    return object;
}

void UEngine::DXRenderer::DXRenderObjectPool::Clear()
{
	while (!creationQueue.empty())
	{
		DXRenderObject::Release(creationQueue.front());
		creationQueue.pop();
	}
	while (!deletionQueue.empty())
	{
		auto obj = deletionQueue.front();
		UINT id = obj->objectID;
		DXRenderObject::Release(obj);
		deletionQueue.pop();
		poolVector.erase(id);
	}
	for (auto objPair : poolVector)
		DXRenderObject::Release(objPair.second);
	poolMap.clear();
	poolVector.clear();
}

void UEngine::DXRenderer::DXRenderObjectPool::OnPreRender()
{
	while (!creationQueue.empty())
	{
		auto obj = creationQueue.front();
		creationQueue.pop();
		if (poolVector[obj->objectID] == obj) continue;
		if (poolVector[obj->objectID] != nullptr)
		{
			poolMap.erase(poolVector[obj->objectID]->objectName);
			DXRenderObject::Release(poolVector[obj->objectID]);
		}
		poolVector[obj->objectID] = obj;
		poolMap[obj->objectName] = obj;
	}
	while (!deletionQueue.empty())
	{
		auto obj = deletionQueue.front();
		deletionQueue.pop();
		poolVector.erase(obj->objectID);
		poolMap.erase(obj->objectName);
		DXRenderObject::Release(obj);
	}
}