#include "dxrframework.h"
#include "DXRenderObjectPool.h"

UEngine::DXRenderer::DXRenderObjectPool::~DXRenderObjectPool()
{
	for (auto object : pool)
		DXRenderObject::Release(object);
	pool.clear();
	while (!creationQueue.empty())
	{
		DXRenderObject::Release(creationQueue.front());
		creationQueue.pop();
	}
	for (auto object : deletionMap)
		DXRenderObject::Release(object.second);
	deletionMap.clear();
}

UEngine::DXRenderer::DXRenderObject* UEngine::DXRenderer::DXRenderObjectPool::LoadObject(std::string shader, std::string renderMesh)
{
    return nullptr;
}

void UEngine::DXRenderer::DXRenderObjectPool::Update()
{
	while (!creationQueue.empty())
	{
		auto curr = creationQueue.front();
		creationQueue.pop();
		pool.push_back(curr);
	}
	auto curr = pool.begin();
	while (curr != pool.end())
	{
		if (*curr == deletionMap[*curr])
			pool.erase(curr);
		curr++;
	}
}
