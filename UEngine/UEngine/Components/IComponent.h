#pragma once

namespace UEngine
{
	class IComponent
	{
	private:
		class GameObject* m_gameObject{ nullptr };

	public:
		IComponent() {};
		virtual ~IComponent() = default;

		GameObject* const gameObject() const { return m_gameObject; }

		template <typename T>
		static T* AddComponent(UEngine::GameObject* const gameObject);
	};

	template<typename T>
	inline T* IComponent::AddComponent(UEngine::GameObject* const gameObject)
	{
		auto component = new T;
		component->m_gameObject = gameObject;
		return component;
	}
}


