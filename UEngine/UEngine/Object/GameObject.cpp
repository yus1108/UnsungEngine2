#include "UEngine.h"
#include "GameObject.h"
#include "Component\IComponent.h"

UEngine::GameObject::GameObject()
{
}

UEngine::GameObject::~GameObject()
{
	for (auto componentListPair : components)
	{
		for (auto component : *componentListPair.second)
			delete component;
		delete componentListPair.second;
	}
}

void UEngine::GameObject::Awake()
{
	for (auto componentListPair : components)
	{
		for (auto component : *componentListPair.second)
		{
			component->Awake();
		}
	}
}

void UEngine::GameObject::Start()
{
	for (auto componentListPair : components)
	{
		for (auto component : *componentListPair.second)
			component->Start();
	}
}

void UEngine::GameObject::OnEnable()
{
	for (auto componentListPair : components)
	{
		for (auto component : *componentListPair.second)
			component->OnEnable();
	}
}

void UEngine::GameObject::FixedUpdate()
{
	for (auto componentListPair : components)
	{
		for (auto component : *componentListPair.second)
			component->FixedUpdate();
	}
}

void UEngine::GameObject::PhysicsUpdate()
{
	for (auto componentListPair : components)
	{
		for (auto component : *componentListPair.second)
			component->PhysicsUpdate();
	}
}

void UEngine::GameObject::Update()
{
	for (auto componentListPair : components)
	{
		for (auto component : *componentListPair.second)
			component->Update();
	}
}

void UEngine::GameObject::LateUpdate()
{
	for (auto componentListPair : components)
	{
		for (auto component : *componentListPair.second)
			component->LateUpdate();
	}
}

void UEngine::GameObject::AnimationUpdate()
{
	for (auto componentListPair : components)
	{
		for (auto component : *componentListPair.second)
			component->AnimationUpdate();
	}
}

void UEngine::GameObject::OnPreRender()
{
	for (auto componentListPair : components)
	{
		for (auto component : *componentListPair.second)
			component->OnPreRender();
	}
}

void UEngine::GameObject::OnRender()
{
	for (auto componentListPair : components)
	{
		for (auto component : *componentListPair.second)
			component->OnRender();
	}
}

void UEngine::GameObject::OnPostRender()
{
	for (auto componentListPair : components)
	{
		for (auto component : *componentListPair.second)
			component->OnPostRender();
	}
}

void UEngine::GameObject::OnDisable()
{
	for (auto componentListPair : components)
	{
		for (auto component : *componentListPair.second)
			component->OnDisable();
	}
}

void UEngine::GameObject::OnDestroy()
{
	for (auto componentListPair : components)
	{
		for (auto component : *componentListPair.second)
			component->OnDestroy();
	}
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
