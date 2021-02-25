#pragma once

namespace UEngine
{
	class GameObject final
	{
		friend class GameState;
	private:
		GameObject() { Awake(); SetActive(true); }
		~GameObject();
		/*
		* 
		*/
		bool isActive{ false };
		bool isStart{ false };
		std::map<std::string, std::list<class IComponent*>*> components;

		void Awake();
		void OnEnable();
		void Start();
		void FixedUpdate();
		void PhysicsUpdate();
		void Update();
		void LateUpdate();
		void AnimationUpdate();
		void OnPreRender();
		void OnPostRender();
		void OnDisable();
		void OnDestroy();

	public:
		void SetActive(bool isActive);
		bool GetActive() { return isActive; }

		template <typename T>
		T* const GetComponent();

		static GameObject* Instantiate();
		static void Release(GameObject** const gameObject);

		template <typename T>
		T* AddComponent();
	};

	template<typename T>
	inline T* const GameObject::GetComponent()
	{
		static_assert(std::is_base_of<IComponent, T>::value, "Provider type does not implement IComponent");

		if (components[typeid(T).raw_name()] == nullptr)
		{
			components.erase(typeid(T).raw_name());
			return nullptr;
		}
		return static_cast<T*>(components[typeid(T).raw_name()]->front());
	}

	template<typename T>
	inline T* GameObject::AddComponent()
	{
		static_assert(std::is_base_of<IComponent, T>::value, "Provider type does not implement IComponent");

		auto component = new T();
		component->gameObject = this;
		static_cast<IComponent*>(component)->Awake();
		component->SetEnable(true);
		if (components[typeid(T).raw_name()] == nullptr) components[typeid(T).raw_name()] = new std::list<IComponent*>();
		components[typeid(T).raw_name()]->push_back(component);
		return component;
	}
}

