#pragma once
#include "Object\Component\Component.h"

namespace UEngine
{
	using namespace Math;
	using namespace Math::Physics2D;
	class Collider : public Component
	{
	private:
		AABB worldAABB{ -0.5f , 0.5f, 0.5f, -0.5f };
		AABB localAABB{ -0.5f , 0.5f, 0.5f, -0.5f };

	public:
		bool isAABBColliding{ false };
		std::map<Collider*, Collider*> others;
		// 충돌체 변수

		AABB GetWorldAABB() { return worldAABB; }

	private:
		void FixedUpdate() override;
		void PhysicsUpdate() override;
	};
}
