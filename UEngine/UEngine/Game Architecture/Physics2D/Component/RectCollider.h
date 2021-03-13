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

		public:
			AABB GetLocalCollider() { return localCollider; }
			AABB GetCollider() { return worldCollider; }
			void SetCollider(float x, float y);

			void RigidBodyUpdate() override;

		private:
			void Awake() override;
			void FixedUpdate() override;

			void CalculateImpact(Collider* other) override;

			static void Calc_Vs_Circle(RectCollider* rect, CircleCollider* circle);
			static void Calc_Vs_Rect(RectCollider* rect1, RectCollider* rect2);
		};
	}
}
