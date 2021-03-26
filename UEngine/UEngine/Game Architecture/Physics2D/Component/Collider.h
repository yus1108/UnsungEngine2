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
			std::unordered_set<Collider*>* prevCollisions;
			std::unordered_set<Collider*>* prevOthers;

			Vector3 impact{ 0, 0, 0 };

			void Start() override;
			void OnDestroy() override;
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
			std::unordered_set<Collider*>* others;
			std::unordered_set<Collider*>* collisions;

			ColliderType GetColliderType() { return colliderType; }
		};
	}
}
