#include "dxrframework.h"
#include "DXRenderObjectPool.h"

UEngine::DXRenderer::DXRenderObjectPool UEngine::DXRenderer::DXRenderObjectPool::instance;

UEngine::DXRenderer::DXRenderObjectPool::~DXRenderObjectPool()
{
	for (auto object : pool)
		DXRenderObject::Release(object);
	pool.clear();
}

UEngine::DXRenderer::DXRenderObject* UEngine::DXRenderer::DXRenderObjectPool::LoadObject(std::string shader, std::string renderMesh, std::list<CONSTANT_BUFFER_DESC> bufferDescs)
{
	auto resourceManager = DXResourceManager::Get();
	auto object = DXRenderObject::Instantiate
	(
		resourceManager->GetRenderMesh(renderMesh),
		resourceManager->GetShaders(shader)
	);
	pool.push_back(object);
    return object;
}

void UEngine::DXRenderer::DXRenderObjectPool::Remove(DXRenderObject* renderObject)
{
	for (auto iter = pool.begin(); iter != pool.end(); iter++)
	{
		if (*iter == renderObject)
		{
			DXRenderObject::Release(*iter);
			pool.erase(iter);
			break;
		}
	}
}
