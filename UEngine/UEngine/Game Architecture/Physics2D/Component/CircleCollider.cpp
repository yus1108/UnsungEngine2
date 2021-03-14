#include "UEngine.h"
#include "CollisionType.h"
#include "CircleCollider.h"


void UEngine::Physics2D::CircleCollider::SetCollider(Vector2 center, float radius)
{
	localCollider = CircleCoord{ center, radius };
	localAABB = AABB{ -radius, radius, radius, -radius };
}

void UEngine::Physics2D::CircleCollider::Awake()
{
	typeName = Type_Circle;
}

void UEngine::Physics2D::CircleCollider::FixedUpdate()
{
	worldCollider = MakeCircle(localCollider.center + GetTransform()->GetWorld().r[3], localCollider.radius);
	Collider::FixedUpdate();
}

void UEngine::Physics2D::CircleCollider::CalculateImpact(Collider* other)
{
	// TODO: need to change to proper collider component
	if (other->GetColliderType() == Type_Circle)
	{
		Calc_Vs_Circle(this, static_cast<CircleCollider*>(other));
	}
	else if (other->GetColliderType() == Type_Rectangle)
	{
		Calc_Vs_Rect(this, static_cast<RectCollider*>(other));
	}
}

void UEngine::Physics2D::CircleCollider::RigidBodyUpdate()
{
	if (GetGameObject()->IsStatic) return;
	auto transform = GetTransform();
	transform->localPosition.value = transform->localPosition.value + impact;
}

void UEngine::Physics2D::CircleCollider::Calc_Vs_Circle(CircleCollider* circle1, CircleCollider* circle2)
{
	if (circle1->IsTrigger || circle2->IsTrigger)
	{
		if (circle1->others.find(circle2) == circle1->others.end())
		{
			if (Math::Physics2D::IsColliding(circle1->GetCollider(), circle2->GetCollider()))
			{
				circle1->others[circle2] = circle2;
				circle2->others[circle1] = circle1;
			}
		}
	}
	else
	{
		if (circle1->collisions.find(circle2) == circle1->collisions.end())
		{
			auto result = FindColliding(circle1, circle2);
			if (result.isColliding)
			{
				if (circle1->GetGameObject()->IsStatic)
					result.distance2 * 2.0f;
				if (circle2->GetGameObject()->IsStatic)
					result.distance1 * 2.0f;

				if (!circle1->GetGameObject()->IsStatic)
					circle1->impact = result.distance1;
				if (!circle2->GetGameObject()->IsStatic)
					circle2->impact = result.distance2;

				circle1->RigidBodyUpdate();
				circle2->RigidBodyUpdate();

				circle1->collisions[circle2] = circle2;
				circle2->collisions[circle1] = circle1;
			}
		}
	}
}

void UEngine::Physics2D::CircleCollider::Calc_Vs_Rect(CircleCollider* circle, RectCollider* rect)
{
	if (circle->IsTrigger || rect->IsTrigger)
	{
		if (circle->others.find(rect) == circle->others.end())
		{
			if (Math::Physics2D::IsColliding(rect->GetCollider(), circle->GetCollider()))
			{
				circle->others[rect] = rect;
				rect->others[circle] = circle;
			}
		}
	}
	else
	{
		if (circle->collisions.find(rect) == circle->collisions.end())
		{
			auto result = FindColliding(rect, circle);
			if (result.isColliding)
			{
				if (circle->GetGameObject()->IsStatic)
					result.distance2 * 2.0f;
				if (rect->GetGameObject()->IsStatic)
					result.distance1 * 2.0f;

				if (!circle->GetGameObject()->IsStatic)
					circle->impact = result.distance1;
				if (!rect->GetGameObject()->IsStatic)
					rect->impact = result.distance2;

				circle->RigidBodyUpdate();
				rect->RigidBodyUpdate();

				circle->collisions[rect] = rect;
				rect->collisions[circle] = circle;
			}
		}
	}
}

