#pragma once
#include "Game Architecture\Object\Component\Component.h"

namespace UEngine
{
	namespace Physics2D
	{
		using namespace Math;
		using namespace Math::Physics2D;
		class Collider : public Component
		{
			friend class SpatialPartition2D;
		private:
			AABB worldAABB{ -0.5f , 0.5f, 0.5f, -0.5f };
			AABB GetWorldAABB() { return worldAABB; }

		protected:
			std::string typeName;
			AABB localAABB{ -0.5f , 0.5f, 0.5f, -0.5f };

			Vector3 impact{ 0, 0, 0 };

			void FixedUpdate() override;
			void PhysicsUpdate() override;

			virtual void CalculateImpact(Collider* other) = 0;
			virtual void RigidBodyUpdate() = 0;

		public:
			bool IsTrigger{ false };
			std::map<Collider*, Collider*> others;
			std::map<Collider*, Collider*> collisions;

			std::string GetColliderType() { return typeName; }
		};
	}
}
