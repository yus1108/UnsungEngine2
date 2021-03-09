#pragma once

namespace UEngine
{
	class GameObject final
	{
		friend class GameState;
		friend class Component;
	private:
		GameObject() { Awake(); SetActive(true); }
		~GameObject();
		/*
		* 
		*/
		bool isActive{ false };
		bool isStart{ false };

		GameObject* parent{ nullptr };
		std::vector<GameObject*> children;

		class Transform* transform{ nullptr };
		class RenderComponent* renderComponent{ nullptr };
		class Material* material{ nullptr };
		std::map<std::string, std::list<class Component*>*> components;

		void Awake();
		void OnEnable();
		void Start();
		void Initialize();
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
		std::string name{ "GameObject" };
		bool IsStatic{ false };

		void SetActive(bool isActive);
		bool GetActive() { return isActive; }

		GameObject* const GetParent() { return parent; }
		GameObject* const GetChild(UINT index) { return children[index]; }
		const std::vector<GameObject*>& GetChildren() { return children; }
		Transform* const GetTransform() { return transform; }

		void SetParent(GameObject* parent);
		void AddChild(GameObject* child);
		void RemoveChild(GameObject* child);

		template <typename T>
		T* const GetComponent();

		template <typename T>
		T* AddComponent();

		template <typename T>
		void RemoveComponent();

		GameObject* const FindObjectWithName(std::string name);

		static GameObject* Instantiate(std::wstring name = L"GameObject");
		static void Release(GameObject** const gameObject);
	};

	template<typename T>
	inline T* const GameObject::GetComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "Provider type does not implement IComponent");

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
		static_assert(std::is_base_of<Component, T>::value, "Provider type does not implement IComponent");

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

		static_cast<Component*>(component)->Awake();
		std::string typeName = typeid(T*).raw_name();
		if (components[typeName] == nullptr) components[typeName] = new std::list<Component*>();
		components[typeName]->push_back(component);
		return component;
	}

	template<typename T>
	inline void GameObject::RemoveComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "Provider type does not implement IComponent");

		if constexpr (std::is_same<T, class Transform>::value)
			throw std::runtime_error("Cannot remove Transform component!");

		std::string typeName = typeid(T*).raw_name();
		if (components[typeName] == nullptr || components[typeName]->size() == 0)
			throw std::runtime_error("doesn't exist in the GameObject!");

		auto component = components[typeName]->back();
		components[typeName]->pop_back();
		component->SetEnable(false);
		static_cast<Component*>(component)->OnDestroy();
		delete component;
		
		if constexpr (std::is_same<T, class RenderComponent>::value)
			renderComponent = nullptr;
		if constexpr (std::is_same<T, class Material>::value)
			material = nullptr;
	}
}

