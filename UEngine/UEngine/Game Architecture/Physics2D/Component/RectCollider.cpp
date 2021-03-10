#include "UEngine.h"
#include "CollisionType.h"
#include "RectCollider.h"

void UEngine::Physics2D::RectCollider::SetCollider(float x, float y)
{
	float halfX = x / 2.0f;
	float halfY = y / 2.0f;
	localCollider = AABB{ -halfX, halfY, halfX, -halfY };
	localAABB = AABB{ -halfX, halfY, halfX, -halfY };
	prevCollider = AABB{ -halfX, halfY, halfX, -halfY };
	prevCollider = MakeAABB(prevCollider, GetTransform()->GetRTP());
	lastPos = GetTransform()->GetWorld().r[3];
}

void UEngine::Physics2D::RectCollider::Awake()
{
	typeName = Type_Rectangle;
	lastPos = GetTransform()->GetWorld().r[3];
	prevCollider = MakeAABB(localAABB, GetTransform()->GetRTP());
}

void UEngine::Physics2D::RectCollider::FixedUpdate()
{
	worldCollider = MakeAABB(localAABB, GetTransform()->GetRTP());
	Collider::FixedUpdate();
}

void UEngine::Physics2D::RectCollider::PhysicsUpdate()
{
	Collider::PhysicsUpdate();
	lastPos = GetTransform()->GetWorld().r[3];
	prevCollider = MakeAABB(localAABB, GetTransform()->GetRTP());
}

void UEngine::Physics2D::RectCollider::CalculateImpact(Collider* other)
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

void UEngine::Physics2D::RectCollider::RigidBodyUpdate()
{
	if (GetGameObject()->IsStatic) return;
	auto transform = GetTransform();
	transform->localPosition = transform->localPosition + impact;
	impact = Vector2();
}

void UEngine::Physics2D::RectCollider::Calc_Vs_Circle(RectCollider* rect, CircleCollider* circle)
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

void UEngine::Physics2D::RectCollider::Calc_Vs_Rect(RectCollider* rect1, RectCollider* rect2)
{
	if (rect1->IsTrigger || rect2->IsTrigger)
	{
		if (rect1->others.find(rect2) == rect1->others.end())
		{
			if (Math::Physics2D::IsColliding(rect1->GetCollider(), rect2->GetCollider()))
			{
				rect1->others[rect2] = rect2;
				rect2->others[rect1] = rect1;
			}
		}
	}
	else
	{
		if (rect1->collisions.find(rect2) == rect1->collisions.end())
		{
			auto result = FindColliding(rect1, rect2);
			if (result.isColliding)
			{
				if (rect1->GetGameObject()->IsStatic)
					result.distance2 * 2.0f;
				if (rect2->GetGameObject()->IsStatic)
					result.distance1 * 2.0f;

				if (!rect1->GetGameObject()->IsStatic)
					rect1->impact = result.distance1;
				if (!rect2->GetGameObject()->IsStatic)
					rect2->impact = result.distance2;

				rect1->RigidBodyUpdate();
				rect2->RigidBodyUpdate();

				rect1->collisions[rect2] = rect2;
				rect2->collisions[rect1] = rect1;
			}
		}
	}
}
