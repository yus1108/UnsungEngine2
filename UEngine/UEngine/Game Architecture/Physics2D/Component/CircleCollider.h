#pragma once
#include "Collider.h"

namespace UEngine
{
	namespace Physics2D
	{
		using namespace Math;
		using namespace Math::Physics2D;
		class CircleCollider final : public Collider
		{
			friend class RectCollider;
		private:
			SERIALIZED_VECTOR3(localCenter);
			SERIALIZED_FLOAT_INIT(localRadius, 0.5f);
			CircleCoord localCollider{ Vector2(), 0.5f };
			CircleCoord worldCollider{ Vector2(), 0.5f };
		public:
			CircleCoord GetCollider() { return worldCollider; }
			void SetCollider(Vector2 center, float radius);

		private:
			void Awake() override;
			void OnEnable() override;
			void FixedUpdate() override;
			void DeSerialize(TiXmlNode* node) override;

			void CalculateImpact(Collider* other) override;
			void RigidBodyUpdate() override;

			static void Calc_Vs_Circle(CircleCollider* circle1, CircleCollider* circle2);
			static void Calc_Vs_Rect(CircleCollider* circle, class RectCollider* rect);
		};
	}
}
