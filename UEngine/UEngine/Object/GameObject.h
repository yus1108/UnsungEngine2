#pragma once

namespace UEngine
{
	class GameObject final
	{
		friend class GameState;
		friend class IComponent;
	private:
		GameObject() { Awake(); SetActive(true); }
		~GameObject();
		/*
		* 
		*/
		bool isActive{ false };
		bool isStart{ false };
		class Transform* transform{ nullptr };
		class RenderComponent* renderComponent{ nullptr };
		class Material* material{ nullptr };
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

		std::string typeName = typeid(T*).raw_name();
		if (components[typeName] == nullptr)
		{
			components.erase(typeName);
			return nullptr;
		}
		return static_cast<T*>(components[typeName]->front());
	}

	template <>
	inline Transform* const GameObject::GetComponent<Transform>()
	{
		return transform;
	}

	template <>
	inline RenderComponent* const GameObject::GetComponent<RenderComponent>()
	{
		std::string typeName = typeid(renderComponent).raw_name();
		if (components[typeName] == nullptr)
		{
			components.erase(typeName);
			return nullptr;
		}
		return renderComponent;
	}

	template <>
	inline Material* const GameObject::GetComponent<Material>()
	{
		std::string typeName = typeid(material).raw_name();
		if (components[typeName] == nullptr)
		{
			components.erase(typeName);
			return nullptr;
		}
		return material;
	}

	template<typename T>
	inline T* GameObject::AddComponent()
	{
		static_assert(std::is_base_of<IComponent, T>::value, "Provider type does not implement IComponent");

		auto component = new T();
		component->gameObject = this;

		if constexpr (std::is_same<T, class Transform>::value)
		{
			if (transform != nullptr) throw std::runtime_error("Cannot add Transform component more than one!");
			transform = component;
		}
		if constexpr (std::is_same<T, class RenderComponent>::value)
		{
			if (renderComponent != nullptr) throw std::runtime_error("Cannot add RenderComponent component more than one!");
			renderComponent = component;
		}
		if constexpr (std::is_same<T, class Material>::value)
		{
			if (material != nullptr) throw std::runtime_error("Cannot add Material component more than one!");
			material = component;
		}

		static_cast<IComponent*>(component)->Awake();
		component->SetEnable(true);
		std::string typeName = typeid(T*).raw_name();
		if (components[typeName] == nullptr) components[typeName] = new std::list<IComponent*>();
		components[typeName]->push_back(component);
		return component;
	}
}

