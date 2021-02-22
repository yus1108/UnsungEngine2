#include "UEngine.h"
#include "GameObject.h"

UEngine::GameObject::GameObject()
	: m_transform(nullptr)
	, renderObject(DXRenderer::DXRenderObject::Instantiate(nullptr, nullptr))
{
	m_transform->AddComponent<Transform>(this);
}

UEngine::GameObject::~GameObject()
{
	for (size_t i = 0; i < components.size(); i++)
	{
		components[i]->SetEnable(false);
		delete components[i];
	}
	DXRenderer::DXRenderObject::Release(renderObject);
	
}

UEngine::GameObject* UEngine::GameObject::Instantiate()
{
	GameObject* obj = new GameObject();
	return obj;
}

void UEngine::GameObject::Release(GameObject** const gameObject)
{
	delete (*gameObject);
	*gameObject = nullptr;
}
