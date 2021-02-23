#pragma once
#include "GameObject.h"

namespace UEngine
{
	class IComponent
	{
		friend class GameObject;
	private:
		virtual void Awake() {}
		virtual void OnEnable() {}
		virtual void OnDisable() {}
		virtual void OnDestroy() {}
		virtual void Update() {}

	private:
		class GameObject* m_gameObject{ nullptr };
		std::string typeName;
		bool enabled{ false };

	public:
		IComponent() {};
		virtual ~IComponent() { OnDestroy(); };

		GameObject* const gameObject() const { return m_gameObject; }

		bool GetEnable() { return enabled; }
		void SetEnable(bool enable)
		{
			this->enabled = enable;
			if (enable) OnEnable();
			else OnDisable();
		}

		void RemoveComponent()
		{
			if(this == m_gameObject->components[0])
			{
				if (m_gameObject->transform()) throw std::runtime_error("Cannot remove Transform component");
			}

			for (size_t i = 0; i < m_gameObject->components.size(); i++)
			{
				if (m_gameObject->components[i]->typeName == typeName)
				{
					m_gameObject->components[i]->SetEnable(false);
					delete m_gameObject->components[i];
					m_gameObject->components.erase(m_gameObject->components.begin() + i);
					return;
				}
			}
			throw std::runtime_error("There is no such component");
		}

		template <typename T>
		static T* AddComponent(GameObject* const gameObject);
		template <typename T>
		static void RemoveComponent(GameObject* const gameObject);
	};

	template<typename T>
	inline T* IComponent::AddComponent(class GameObject* const gameObject)
	{
		static_assert(std::is_base_of<IComponent, T>::value, "Provider type does not implement IComponent");
		if constexpr (std::is_same<T, class Transform>::value)
		{
			if (gameObject->transform()) throw std::runtime_error("Transform component can be only one for each GameObject");
		}

		auto component = new T;
		component->m_gameObject = gameObject;
		component->typeName = typeid(T).name();
		component->Awake();
		component->SetEnable(true);
		gameObject->components.emplace_back(component);
		return component;
	}

	template<typename T>
	inline void IComponent::RemoveComponent(UEngine::GameObject* const gameObject)
	{
		static_assert(std::is_base_of<IComponent, T>::value, "Provider type does not implement IComponent");
		if constexpr (std::is_same<T, class Transform>::value)
		{
			if (gameObject->transform()) throw std::runtime_error("Cannot remove Transform component");
		}
		
		for (size_t i = 0; i < gameObject->components.size(); i++)
		{
			if (gameObject->components[i]->typeName == typeid(T).name())
			{
				gameObject->components[i]->SetEnable(false);
				delete gameObject->components[i];
				gameObject->components.erase(gameObject->components.begin() + i);
				return;
			}
		}
		throw std::runtime_error("There is no such component");
	}
}