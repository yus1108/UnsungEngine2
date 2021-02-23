#pragma once
#include "Components\IComponent.h"

namespace UEngine
{
	class GameObject final
	{
		friend class GameState;
		friend class RenderMesh;
		friend class Shader;
		friend class Material;
		friend class Camera;
		friend class Transform;
	private:
		GameObject();
		~GameObject();

	private:
		class Transform* m_transform;
		DXRenderer::DXRenderObject* const renderObject;

		void CopyRenderObject(DXRenderer::DXRenderObject* renderObject);
		DXRenderer::DXRenderObject* const GetRenderObject() { return renderObject; }

		void Update();

	public:
		GameObject* parent{ nullptr };
		std::vector<GameObject*> children;
		std::vector<class IComponent*> components;
		
	public:
		Transform* const transform() { return m_transform; }

		static GameObject* Instantiate();
		static void Release(GameObject** const gameObject);

		template <typename T>
		T* AddComponent();
		template <typename T>
		void RemoveComponent();
	};

	template<typename T>
	inline T* GameObject::AddComponent()
	{
		auto component = IComponent::AddComponent<T>(this);
		return component;
	}

	template<typename T>
	inline void GameObject::RemoveComponent()
	{
		IComponent::RemoveComponent<T>(this);
	}
}

