#pragma once
#include "Collider.h"

namespace UEngine
{
	namespace Physics2D
	{
		using namespace Math;
		using namespace Math::Physics2D;
		class RectCollider final : public Collider
		{
			friend class CircleCollider;
		private:
			SERIALIZED_FLOAT_INIT(left, -0.5f);
			SERIALIZED_FLOAT_INIT(top, 0.5f);
			SERIALIZED_FLOAT_INIT(right, 0.5f);
			SERIALIZED_FLOAT_INIT(bottom, -0.5f);
			AABB localCollider{ -0.5f, 0.5f, 0.5f, -0.5f };
			AABB worldCollider{ -0.5f, 0.5f, 0.5f, -0.5f };

		public:
			AABB GetLocalCollider() { return localCollider; }
			AABB GetCollider() { return worldCollider; }
			void SetCollider(float x, float y);
			void SetCollider(float left, float bottom, float right, float top);

			void RigidBodyUpdate() override;

		private:
			void Awake() override;
			void FixedUpdate() override;
			void DeSerialize(TiXmlNode* node) override;

			void CalculateImpact(Collider* other) override;

			static void Calc_Vs_Circle(RectCollider* rect, CircleCollider* circle);
			static void Calc_Vs_Rect(RectCollider* rect1, RectCollider* rect2);
		};
	}
}
