#pragma once
#include "Game Architecture\Object\Component\Component.h"
#include "CollisionType.h"

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
			ColliderType colliderType;
			AABB localAABB{ -0.5f , 0.5f, 0.5f, -0.5f };
			std::vector<Collider*>* prevCollisions;
			std::vector<Collider*>* prevOthers;

			Vector3 impact{ 0, 0, 0 };

			Collider();
			virtual ~Collider();

			static bool VectorFind(std::vector<Collider*>* vColliders, Collider* collider);

			void OnEnable() override;
			void FixedUpdate() override;
			void PhysicsUpdate() override;
			void Update() override;

			virtual void CalculateImpact(Collider* other) = 0;
			virtual void RigidBodyUpdate() = 0;
			void OnTrigger(Collider* other);
			void OnCollision(Collider* other);

		public:
			SERIALIZED_BOOL(IsTrigger);
			std::vector<Collider*>* others;
			std::vector<Collider*>* collisions;

			ColliderType GetColliderType() { return colliderType; }
		};
	}
}
