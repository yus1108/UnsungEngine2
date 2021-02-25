#include "UEngine.h"
#include "GameObject.h"
#include "Component\Component.h"

UEngine::GameObject::~GameObject()
{
	OnDisable();
	OnDestroy();
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

void UEngine::GameObject::OnEnable()
{
	for (auto componentListPair : components)
	{
		for (auto component : *componentListPair.second)
			component->OnEnable();
	}
}

void UEngine::GameObject::Start()
{
	if (isStart) return;
	for (auto componentListPair : components)
	{
		for (auto component : *componentListPair.second)
			component->Start();
	}
	isStart = true;
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

void UEngine::GameObject::SetActive(bool isActive)
{
	if (isActive && !this->isActive)
	{
		OnEnable();
		Start();
	}
	else if (!isActive && this->isActive)
	{
		OnDisable();
	}
	this->isActive = isActive;
}

void UEngine::GameObject::SetParent(GameObject* parent)
{
	if (this->parent != nullptr) this->parent->RemoveChild(this);
	if (parent != nullptr) parent->children.push_back(this);
	this->parent = parent;
}

void UEngine::GameObject::AddChild(GameObject* child)
{
	child->SetParent(this);
}

void UEngine::GameObject::RemoveChild(GameObject* child)
{
	for (auto iter = children.begin(); iter != children.end(); iter++)
	{
		if (*iter == child)
		{
			children.erase(iter);
			return;
		}
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
