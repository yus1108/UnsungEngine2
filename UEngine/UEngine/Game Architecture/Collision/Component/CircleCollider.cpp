#include "UEngine.h"
#include "CircleCollider.h"

void UEngine::CircleCollider::SetCollider(Vector2 center, float radius)
{
	localCollider = CircleCoord{ center, radius };
	localAABB = AABB{ -radius, radius, radius, -radius };
}

void UEngine::CircleCollider::Awake()
{
	typeName = typeid(this).raw_name();
}

void UEngine::CircleCollider::FixedUpdate()
{
	worldCollider = MakeCircle(localCollider.center + GetTransform()->GetWorld().r[3], localCollider.radius);
	Collider::FixedUpdate();
}
