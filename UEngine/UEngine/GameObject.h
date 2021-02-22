#pragma once
#include "Components\IComponent.h"

namespace UEngine
{
	class GameObject final
	{
		friend class RenderMesh;
		friend class Shader;
	private:
		GameObject();
		~GameObject();

	private:
		class Transform* m_transform;
		DXRenderer::DXRenderObject* const renderObject;

	public:
		std::vector<GameObject*> children;
		std::vector<class IComponent*> components;
		
	public:
		Transform* const transform() { return m_transform; }

		static GameObject* Instantiate();
		static void Release(GameObject** const gameObject);

		template <typename T>
		void RemoveComponent();
	};

	template<typename T>
	inline void GameObject::RemoveComponent()
	{
		IComponent::RemoveComponent<T>(this);
	}
}

