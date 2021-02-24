#pragma once

namespace UEngine
{
	class GameObject final
	{
	private:
		GameObject();
		~GameObject();
		/*
		* 
		*/
		std::map<std::string, std::list<IComponent*>*> components;

	public:
		static GameObject* Instantiate();
		static void Release(GameObject** const gameObject);

		template <typename T>
		T* AddComponent();
	};

	template<typename T>
	inline T* GameObject::AddComponent()
	{
		static_assert(std::is_base_of<IComponent, T>::value, "Provider type does not implement IComponent");

		auto component = new T();
		if (components[typeid(T).raw_name()] == nullptr) components[typeid(T).raw_name()] = new std::list<IComponent*>();
		components[typeid(T).raw_name()]->push_back(component);
		return component;
	}
}

