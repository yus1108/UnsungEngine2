#pragma once

namespace UEngine
{
	using namespace Math;
	using namespace Math::Physics2D;
	class CircleCollider : public Collider
	{
	private:
		CircleCoord localCollider{ Vector2(), 0.5f };
		CircleCoord worldCollider{ Vector2(), 0.5f };
	public:
		CircleCoord GetCollider() { return worldCollider; }
		void SetCollider(Vector2 center, float radius);

	private:
		void Awake() override;
		void FixedUpdate() override;
	};
}