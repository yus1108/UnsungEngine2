#include "UEngine.h"
#include "GameObject.h"

UEngine::GameObject::GameObject()
	: transform(IComponent::AddComponent<Transform>(this))
	, renderObject(DXRenderer::DXRenderObject::Instantiate(nullptr, nullptr))
{
}

UEngine::GameObject::~GameObject()
{
	DXRenderer::DXRenderObject::Release(renderObject);
}

UEngine::GameObject* UEngine::GameObject::Instantiate()
{
	return new GameObject();
}
