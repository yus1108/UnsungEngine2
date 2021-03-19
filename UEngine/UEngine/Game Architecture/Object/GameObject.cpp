#include "UEngine.h"
#include "GameObject.h"
#include "Component\Component.h"

UEngine::GameObject::~GameObject()
{
	OnDisable();
	OnDestroy();
	for (size_t i = 0; i < components.size(); i++)
		delete components[i];
	components.clear();
	for (size_t i = 0; i < children.size(); i++)
		GetScene()->RemoveGameObject(&children[i]);
	if (parent != nullptr)
		parent->RemoveChild(this);
	children.clear();
}

std::string UEngine::GameObject::GetComponentTypeName(Component* component)
{
	return component->typeName;
}

void UEngine::GameObject::SetComponentTypeName(Component* component, std::string typeName)
{
	component->typeName = typeName;
}

void UEngine::GameObject::RemoveComponent(Component* component)
{
	component->SetEnable(false);
	component->OnDestroy();
}

void UEngine::GameObject::Awake()
{
	for (size_t i = 0; i < components.size(); i++)
		components[i]->Awake();
}

void UEngine::GameObject::OnEnable()
{
	for (size_t i = 0; i < components.size(); i++)
		components[i]->SetEnable(true);
}

void UEngine::GameObject::Initialize()
{
	for (size_t i = 0; i < components.size(); i++)
		components[i]->Initialize();
}

void UEngine::GameObject::FixedUpdate()
{
	for (size_t i = 0; i < components.size(); i++)
		if (components[i]->GetEnable()) components[i]->FixedUpdate();
}

void UEngine::GameObject::PhysicsUpdate()
{
	for (size_t i = 0; i < components.size(); i++)
		if (components[i]->GetEnable()) components[i]->PhysicsUpdate();
}

void UEngine::GameObject::Update()
{
	for (size_t i = 0; i < components.size(); i++)
		if (components[i]->GetEnable()) components[i]->Update();
}

void UEngine::GameObject::LateUpdate()
{
	for (size_t i = 0; i < components.size(); i++)
		if (components[i]->GetEnable()) components[i]->LateUpdate();
}

void UEngine::GameObject::AnimationUpdate()
{
	for (size_t i = 0; i < components.size(); i++)
		if (components[i]->GetEnable()) components[i]->AnimationUpdate();
}

void UEngine::GameObject::OnPreRender()
{
	for (size_t i = 0; i < components.size(); i++)
		if (components[i]->GetEnable() || components[i]->typeName == ".PAVEditorScript@UEngine@@") components[i]->OnPreRender();
}

void UEngine::GameObject::OnPostRender()
{
	for (size_t i = 0; i < components.size(); i++)
		if (components[i]->GetEnable()) components[i]->OnPostRender();
}

void UEngine::GameObject::OnDisable()
{
	for (size_t i = 0; i < components.size(); i++)
		components[i]->SetEnable(false);
}

void UEngine::GameObject::OnDestroy()
{
	for (size_t i = 0; i < components.size(); i++)
		components[i]->OnDestroy();
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

UEngine::GameObject* const UEngine::GameObject::FindObjectWithName(std::string name)
{
	return scene->GetGameObject(name);
}

UEngine::GameObject* UEngine::GameObject::Instantiate(std::string name)
{
	UEngine::GameObject* obj = new UEngine::GameObject();
	obj->name = name;
	obj->scene = GameState::GetCurrentScene();
	obj->scene->AddGameObject(obj);
	obj->AddComponent<Transform>();
	return obj;
}

UEngine::GameObject* UEngine::GameObject::Instantiate(GameScene* scene, std::string name)
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
