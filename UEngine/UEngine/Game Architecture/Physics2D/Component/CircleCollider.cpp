#include "UEngine.h"
#include "CollisionType.h"
#include "CircleCollider.h"


void UEngine::Physics2D::CircleCollider::SetCollider(Vector2 center, float radius)
{
	localCenter = center;
	localRadius = radius;
	localCollider = CircleCoord{ center, radius };
	localAABB = AABB{ center.x - radius, center.y + radius, center.x + radius, center.y + -radius };
}

void UEngine::Physics2D::CircleCollider::DeSerialize(TiXmlNode* node)
{
	Collider::DeSerialize(node);
	SetCollider(localCenter.value, localRadius.value);
}

void UEngine::Physics2D::CircleCollider::Awake()
{
	colliderType = ColliderType::Circle;
}

void UEngine::Physics2D::CircleCollider::OnEnable()
{
	worldCollider = MakeCircle(localCollider.center + GetTransform()->GetWorld().r[3], localCollider.radius);
	Collider::OnEnable();
}

void UEngine::Physics2D::CircleCollider::FixedUpdate()
{
	if (!GetGameObject()->IsStatic)
		worldCollider = MakeCircle(localCollider.center + GetTransform()->GetWorld().r[3], localCollider.radius);
	Collider::FixedUpdate();
}

void UEngine::Physics2D::CircleCollider::CalculateImpact(Collider* other)
{
	// TODO: need to change to proper collider component
	if (other->GetColliderType() == ColliderType::Circle)
	{
		Calc_Vs_Circle(this, static_cast<CircleCollider*>(other));
	}
	else if (other->GetColliderType() == ColliderType::AABB)
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
	if (circle1->IsTrigger.value || circle2->IsTrigger.value)
	{
		if (circle1->others.find(circle2) == circle1->others.end())
		{
			if (Math::Physics2D::IsColliding(circle1->GetCollider(), circle2->GetCollider()))
			{
				circle1->OnTrigger(circle2);
				circle2->OnTrigger(circle1);
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

				circle1->OnCollision(circle2);
				circle2->OnCollision(circle1);
			}
		}
	}
}

void UEngine::Physics2D::CircleCollider::Calc_Vs_Rect(CircleCollider* circle, RectCollider* rect)
{
	if (circle->IsTrigger.value || rect->IsTrigger.value)
	{
		if (circle->others.find(rect) == circle->others.end())
		{
			if (Math::Physics2D::IsColliding(rect->GetCollider(), circle->GetCollider()))
			{
				circle->OnTrigger(rect);
				rect->OnTrigger(circle);
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

				circle->OnCollision(rect);
				rect->OnCollision(circle);
			}
		}
	}
}

