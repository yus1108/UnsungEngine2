#pragma once

namespace UEngine
{
	class GameObject final
	{
		friend class GameScene;
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

		class GameScene* scene{ nullptr };
		class Transform* transform{ nullptr };
		class RenderComponent* renderComponent{ nullptr };
		class Material* material{ nullptr };
		std::vector<class Component*> components, deletionComponents;

		std::string GetComponentTypeName(Component* component);
		void SetComponentTypeName(Component* component, std::string typeName);
		void DisableComponent(Component* component);

		void Awake();
		void OnEnable();
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
		void Sync();

		void RemoveChild(GameObject* child);
	public:
		std::string name{ "GameObject" };
		bool IsStatic{ false };

		void SetActive(bool isActive);
		bool GetActive() { return isActive; }

		GameScene* const GetScene() { return scene; }
		GameObject* const GetParent() { return parent; }
		GameObject* const GetChild(UINT index) { return children[index]; }
		const std::vector<GameObject*>& GetChildren() { return children; }
		Transform* const GetTransform() { return transform; }

		void SetParent(GameObject* parent);
		void AddChild(GameObject* child);

		template <typename T>
		T* const GetComponent();

		const std::vector<class Component*> GetComponents() { return components; }

		template <typename T>
		T* AddComponent();

		template <typename T>
		void RemoveComponent();
		void RemoveComponent(Component* component);

		GameObject* const FindObjectWithName(std::string name);

		static GameObject* Instantiate(std::string name = "GameObject");
		static GameObject* Instantiate(GameScene* scene, std::string name = "GameObject");
		static void Release(GameObject** const gameObject);
	};

	template<typename T>
	inline T* const GameObject::GetComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "Provider type does not implement IComponent");

		std::string typeName = typeid(T*).raw_name();
		for (size_t i = 0; i < components.size(); i++)
		{
			if (GetComponentTypeName(components[i]) == typeName)
				return static_cast<T*>(components[i]);
		}
		return nullptr;
	}

	template <>
	inline Transform* const GameObject::GetComponent<Transform>()
	{
		return transform;
	}

	template <>
	inline RenderComponent* const GameObject::GetComponent<RenderComponent>()
	{
		if (renderComponent != nullptr) return renderComponent;
		return nullptr;
	}

	template <>
	inline Material* const GameObject::GetComponent<Material>()
	{
		if (material != nullptr) return material;
		return nullptr;
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
		SetComponentTypeName(component, typeid(T*).raw_name());
		components.emplace_back(component);
		return component;
	}

	template<typename T>
	inline void GameObject::RemoveComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "Provider type does not implement IComponent");

		if constexpr (std::is_same<T, class Transform>::value)
			throw std::runtime_error("Cannot remove Transform component!");

		std::string typeName = typeid(T*).raw_name();
		for (size_t i = 0; i < components.size(); i++)
		{
			if (GetComponentTypeName(components[i]) == typeName)
			{
				auto component = components[i];
				DisableComponent(component);
				deletionComponents.emplace_back(component);

				if constexpr (std::is_same<T, class RenderComponent>::value)
					renderComponent = nullptr;
				if constexpr (std::is_same<T, class Material>::value)
					material = nullptr;

				return;
			}
		}
		throw std::runtime_error("doesn't exist in the GameObject!");
	}
}

