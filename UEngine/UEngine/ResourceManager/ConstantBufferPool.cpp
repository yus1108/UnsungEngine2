#include "UEngine.h"
#include "ConstantBufferPool.h"

UEngine::ConstantBufferPool::~ConstantBufferPool()
{
	while (!creationQueue.empty())
	{
		DXRenderer::DXConstantBuffer::Release(&creationQueue.front());
		creationQueue.pop();
	}
	for (auto objectPair : deletionMap)
	{
		auto objectToDelete = objectPair.second;
		pool.erase(objectToDelete);
		DXRenderer::DXConstantBuffer::Release(&objectPair.second);
	}
	for (auto object : pool)
	{
		DXRenderer::DXConstantBuffer::Release(&object.second);
	}

}

void UEngine::ConstantBufferPool::OnPreRender()
{
	while (!creationQueue.empty())
	{
		auto curr = creationQueue.front();
		creationQueue.pop();
		pool[curr] = curr;
	}
	for (auto objectPair : deletionMap)
	{
		auto objectToDelete = objectPair.second;
		pool.erase(objectToDelete);
		DXRenderer::DXConstantBuffer::Release(&objectToDelete);
	}
	deletionMap.clear();
}
