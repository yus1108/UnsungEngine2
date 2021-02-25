#pragma once
#include "Object\GameObject.h"
#include <iostream>
namespace UEngine
{
	class Component
	{
	private:
		friend class UEngine::GameObject;

	private:
		GameObject* gameObject{ nullptr };
		bool enabled{ false };
		bool isStart{ false };

	protected:
		GameObject* const GetGameObject() { return gameObject; }
		Transform* const GetTransform() { return gameObject->GetTransform(); }

	private:
		virtual void Awake() {}
		virtual void Start() {}
		virtual void OnEnable() {}
		virtual void FixedUpdate() {}
		virtual void PhysicsUpdate() {}
		virtual void Update() {}
		virtual void LateUpdate() {}
		virtual void AnimationUpdate() {}
		virtual void OnPreRender() {}
		virtual void OnPostRender() {}
		virtual void OnDisable() {}
		virtual void OnDestroy() {}

	public:
		void SetEnable(bool enable)
		{
			if (gameObject->GetActive())
			{
				if (enable && !enabled)
				{
					OnEnable();
					if (isStart) return;
					Start();
					isStart = true;
				}
				else if (!enable && enabled) OnDisable();
			}
			enabled = enable;
		}
		bool GetEnable() { return enabled; }

		template <typename T>
		T* const GetComponent();

		template <typename T>
		T* AddComponent();

		template <typename T>
		void RemoveComponent();

		Component() = default;
		virtual ~Component() { SetEnable(false); OnDestroy(); };
	};
	template<typename T>
	inline T* const Component::GetComponent()
	{
		return gameObject->GetComponent<T>();
	}
	template<typename T>
	inline T* Component::AddComponent()
	{
		return gameObject->AddComponent<T>();
	}
	template<typename T>
	inline void Component::RemoveComponent()
	{
		gameObject->RemoveComponent<T>();
	}
}