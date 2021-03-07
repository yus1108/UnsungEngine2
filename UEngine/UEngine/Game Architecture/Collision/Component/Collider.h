#pragma once
#include "Object\Component\Component.h"

namespace UEngine
{
	using namespace Math;
	using namespace Math::Physics2D;
	class Collider : public Component
	{
		friend class SpatialPartition2D;
	private:
		Vector3 dir{ 0, 0, 0 };
		AABB worldAABB{ -0.5f , 0.5f, 0.5f, -0.5f };

		AABB GetWorldAABB() { return worldAABB; }
		void PhysicsUpdate() override;
		void RigidBodyUpdate();

	protected:
		std::string typeName;
		AABB localAABB{ -0.5f , 0.5f, 0.5f, -0.5f };

		void FixedUpdate() override;

	public:
		bool IsTrigger{ false };
		std::map<Collider*, Collider*> others;
		std::map<Collider*, Collider*> collisions;

		std::string GetColliderType() { return typeName; }
	};
}
