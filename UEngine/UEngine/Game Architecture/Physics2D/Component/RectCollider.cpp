#include "UEngine.h"
#include "CollisionType.h"
#include "RectCollider.h"

void UEngine::Physics2D::RectCollider::SetCollider(float x, float y)
{
	float halfX = x / 2.0f;
	float halfY = y / 2.0f;
	this->left = -halfX;
	this->bottom = -halfY;
	this->right = halfX;
	this->top = halfY;
	localCollider = AABB{ -halfX, halfY, halfX, -halfY };
	localAABB = AABB{ -halfX, halfY, halfX, -halfY };
}

void UEngine::Physics2D::RectCollider::SetCollider(float left, float bottom, float right, float top)
{
	this->left = left;
	this->bottom = bottom;
	this->right = right;
	this->top = top;
	localCollider = AABB{ left, top, right, bottom };
	localAABB = AABB{ left, top, right, bottom };
}

void UEngine::Physics2D::RectCollider::Awake()
{
	colliderType = ColliderType::AABB;
}

void UEngine::Physics2D::RectCollider::OnEnable()
{
	worldCollider = MakeAABB(localAABB, GetTransform()->GetRTP());
	Collider::OnEnable();
}

void UEngine::Physics2D::RectCollider::FixedUpdate()
{
	if (!GetGameObject()->IsStatic)
		worldCollider = MakeAABB(localAABB, GetTransform()->GetRTP());
	Collider::FixedUpdate();
}

void UEngine::Physics2D::RectCollider::DeSerialize(TiXmlNode* node)
{
	Collider::DeSerialize(node);
	SetCollider(left.value, bottom.value, right.value, top.value);
}

void UEngine::Physics2D::RectCollider::CalculateImpact(Collider* other)
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

void UEngine::Physics2D::RectCollider::RigidBodyUpdate()
{
	if (GetGameObject()->IsStatic) return;
	auto transform = GetTransform();
	transform->localPosition.value = transform->localPosition.value + impact;
	impact = Vector2();
}

void UEngine::Physics2D::RectCollider::Calc_Vs_Circle(RectCollider* rect, CircleCollider* circle)
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

void UEngine::Physics2D::RectCollider::Calc_Vs_Rect(RectCollider* rect1, RectCollider* rect2)
{
	if (rect1->IsTrigger.value || rect2->IsTrigger.value)
	{
		if (rect1->others.find(rect2) == rect1->others.end())
		{
			if (Math::Physics2D::IsColliding(rect1->GetCollider(), rect2->GetCollider()))
			{
				rect1->OnTrigger(rect2);
				rect2->OnTrigger(rect1);
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

				rect1->OnCollision(rect2);
				rect2->OnCollision(rect1);
			}
		}
	}
}
