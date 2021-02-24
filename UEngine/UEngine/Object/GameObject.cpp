#include "UEngine.h"
#include "GameObject.h"

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
