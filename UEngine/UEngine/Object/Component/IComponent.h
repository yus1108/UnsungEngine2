#pragma once
#include "Object\GameObject.h"

namespace UEngine
{
	class IComponent
	{
	private:
		friend class UEngine::GameObject;

	private:
		GameObject* gameObject;

	protected:
		GameObject* const GetGameObject() { return gameObject; }

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
		virtual void OnRender() {}
		virtual void OnPostRender() {}
		virtual void OnDisable() {}
		virtual void OnDestroy() {}

	public:
		IComponent() { Awake(); Start(); };
		virtual ~IComponent() { OnDestroy(); };
	};
}