#pragma once

namespace UEngine
{
	class GameObject final
	{
		friend class GameState;
	private:
		GameObject();
		~GameObject();
		/*
		* 
		*/
		std::map<std::string, std::list<class IComponent*>*> components;

		void Awake();
		void Start();
		void OnEnable();
		void FixedUpdate();
		void PhysicsUpdate();
		void Update();
		void LateUpdate();
		void AnimationUpdate();
		void OnPreRender();
		void OnRender();
		void OnPostRender();
		void OnDisable();
		void OnDestroy();

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
		component->gameObject = this;
		if (components[typeid(T).raw_name()] == nullptr) components[typeid(T).raw_name()] = new std::list<IComponent*>();
		components[typeid(T).raw_name()]->push_back(component);
		return component;
	}
}

