#include "UEngine.h"
#include "GameObject.h"

UEngine::GameObject::GameObject()
	: renderObject(DXRenderer::DXRenderObject::Instantiate(nullptr, nullptr))
	, m_transform(nullptr)
{
	m_transform = m_transform->AddComponent<Transform>(this);
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

void UEngine::GameObject::CopyRenderObject(DXRenderer::DXRenderObject* renderObject)
{
	this->renderObject->SetShader(renderObject->GetShader());
	this->renderObject->SetRenderMesh(renderObject->GetRenderMesh());
	this->renderObject->SetConstantBuffers(renderObject->GetConstantBuffers());
}

void UEngine::GameObject::Update()
{
	for (auto component : components)
		component->Update();
}

UEngine::GameObject* UEngine::GameObject::Instantiate()
{
	UEngine::GameObject* obj = new UEngine::GameObject();
	return obj;
}

void UEngine::GameObject::Release(GameObject** const gameObject)
{
	delete (*gameObject);
	*gameObject = nullptr;
}
