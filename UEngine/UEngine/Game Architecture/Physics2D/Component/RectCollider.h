#pragma once

namespace UEngine
{
	namespace Physics2D
	{
		using namespace Math;
		using namespace Math::Physics2D;
		class RectCollider : public Collider
		{
			friend class CircleCollider;
		private:
			AABB localCollider{ -0.5f, 0.5f, 0.5f, -0.5f };
			AABB worldCollider{ -0.5f, 0.5f, 0.5f, -0.5f };
			Vector2 lastPos;
		public:
			AABB GetCollider() { return worldCollider; }
			void SetCollider(float x, float y);

			Vector2 GetLastPos() { return lastPos; }

		private:
			void Awake() override;
			void FixedUpdate() override;
			void PhysicsUpdate() override;

			void CalculateImpact(Collider* other) override;
			void RigidBodyUpdate() override;

			static void Calc_Vs_Circle(RectCollider* rect, CircleCollider* circle);
			static void Calc_Vs_Rect(RectCollider* rect1, RectCollider* rect2);
		};
	}
}
