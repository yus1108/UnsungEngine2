#include "pch.h"
#include "Enemy.h"

void Enemy::Start()
{
	transform = GetTransform();
	collider = GetComponent<Physics2D::CircleCollider>();
}

void Enemy::Update()
{
	deltaTime = Utility::UTime::Get()->DeltaTimeF();
	velocity = velocity + gravity * deltaTime;
	Console::Clear();
	Console::WriteLine(to_string(transform->localPosition.value.x));
}

void Enemy::LateUpdate()
{
	transform->localPosition.value = transform->localPosition.value + velocity;
}

void Enemy::OnTriggerEnter(Physics2D::Collider* other)
{
	if (other->GetGameObject()->name == "tile")
	{
		auto result = Math::Physics2D::FindColliding(static_cast<Physics2D::RectCollider*>(other), collider);
		transform->localPosition.value = transform->localPosition.value + result.distance1 * 2.0f;
		velocity = Vector2();
	}
	
}

void Enemy::OnTriggerStay(Physics2D::Collider* other)
{
	if (other->GetGameObject()->name == "tile")
	{
		auto result = Math::Physics2D::FindColliding(static_cast<Physics2D::RectCollider*>(other), collider);
		transform->localPosition.value = transform->localPosition.value + result.distance1 * 2.0f;
		velocity = Vector2();
	}
}

void Enemy::OnTriggerExit(Physics2D::Collider* other)
{

}
