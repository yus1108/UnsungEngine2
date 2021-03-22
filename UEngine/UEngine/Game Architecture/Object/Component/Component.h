#pragma once
#include "Game Architecture\Object\GameObject.h"

namespace UEngine
{
	class EditorScript;
	namespace Physics2D
	{
		class Collider;
	}
	class Component : public Serializer
	{
	private:
		friend class UEngine::GameObject;
		friend class UEngine::EditorScript;
		friend class Physics2D::Collider;
	private:
		GameObject* gameObject{ nullptr };
		bool initialized{ false };
		bool enabled{ true };
		bool isStart{ false };

	private:
		void Initialize() 
		{
			if (!initialized)
			{
				if (enabled)
				{
					OnEnable();
					if (!isStart)
					{
						Start();
						isStart = true;
					}
				}
			}
			initialized = true;
		}
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

		virtual void OnTriggerEnter(Physics2D::Collider* other) {}
		virtual void OnTriggerStay(Physics2D::Collider* other) {}
		virtual void OnTriggerExit(Physics2D::Collider* other) {}
		virtual void OnCollisionEnter(Physics2D::Collider* collision) {}
		virtual void OnCollisionStay(Physics2D::Collider* collision) {}
		virtual void OnCollisionExit(Physics2D::Collider* collision) {}

	public:
		std::string typeName;

		GameObject* const GetGameObject() { return gameObject; }
		Transform* const GetTransform() { return gameObject->GetTransform(); }

		virtual void SetEnable(bool enable)
		{
			if (gameObject->GetActive() && initialized)
			{
				if (enable && !enabled)
				{
					enabled = true;
					OnEnable();
					if (!isStart)
					{
						Start();
						isStart = true;
					}
				}
				else if (!enable && enabled)
				{
					enabled = false;
					OnDisable();
				}
			}
			else
				enabled = enable;
		}
		bool GetEnable() { return enabled; }

		template <typename T>
		T* const GetComponent();

		template <typename T>
		T* AddComponent();

		template <typename T>
		void RemoveComponent();

		GameObject* const FindObjectWithName(std::string name) { return gameObject->FindObjectWithName(name); }

		//Component() = default;
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