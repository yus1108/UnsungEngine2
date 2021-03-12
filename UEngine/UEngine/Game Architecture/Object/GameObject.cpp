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
			component->SetEnable(true);
	}
}

void UEngine::GameObject::Initialize()
{
	for (auto componentListPair : components)
	{
		for (auto component : *componentListPair.second)
			component->Initialize();
	}
}

void UEngine::GameObject::FixedUpdate()
{
	for (auto componentListPair : components)
	{
		for (auto component : *componentListPair.second)
			if (component->GetEnable()) component->FixedUpdate();
	}
}

void UEngine::GameObject::PhysicsUpdate()
{
	for (auto componentListPair : components)
	{
		for (auto component : *componentListPair.second)
			if (component->GetEnable()) component->PhysicsUpdate();
	}
}

void UEngine::GameObject::Update()
{
	for (auto componentListPair : components)
	{
		for (auto component : *componentListPair.second)
			if (component->GetEnable()) component->Update();
	}
}

void UEngine::GameObject::LateUpdate()
{
	for (auto componentListPair : components)
	{
		for (auto component : *componentListPair.second)
			if (component->GetEnable()) component->LateUpdate();
	}
}

void UEngine::GameObject::AnimationUpdate()
{
	for (auto componentListPair : components)
	{
		for (auto component : *componentListPair.second)
			if (component->GetEnable()) component->AnimationUpdate();
	}
}

void UEngine::GameObject::OnPreRender()
{
	for (auto componentListPair : components)
	{
		for (auto component : *componentListPair.second)
			if (component->GetEnable()) component->OnPreRender();
	}
}

void UEngine::GameObject::OnRender()
{
	for (auto componentListPair : components)
	{
		for (auto component : *componentListPair.second)
			if (component->GetEnable()) component->OnRender();
	}
}

void UEngine::GameObject::OnPostRender()
{
	for (auto componentListPair : components)
	{
		for (auto component : *componentListPair.second)
			if (component->GetEnable()) component->OnPostRender();
	}
}

void UEngine::GameObject::OnDisable()
{
	for (auto componentListPair : components)
	{
		for (auto component : *componentListPair.second)
			component->SetEnable(false);
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

UEngine::GameObject* const UEngine::GameObject::FindObjectWithName(std::wstring name)
{
	return scene->GetGameObject(name);
}

UEngine::GameObject* UEngine::GameObject::Instantiate(std::wstring name)
{
	UEngine::GameObject* obj = new UEngine::GameObject();
	obj->name = name;
	obj->scene = GameState::GetCurrentScene();
	obj->scene->AddGameObject(obj);
	obj->AddComponent<Transform>();
	return obj;
}

UEngine::GameObject* UEngine::GameObject::Instantiate(GameScene* scene, std::wstring name)
{
	UEngine::GameObject* obj = new UEngine::GameObject();
	obj->name = name;
	obj->scene = scene;
	obj->scene->AddGameObject(obj);
	obj->AddComponent<Transform>();
	return obj;
}

void UEngine::GameObject::Release(GameObject** const gameObject)
{
	delete (*gameObject);
	*gameObject = nullptr;
}
