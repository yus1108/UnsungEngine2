#include "pch.h"
#include "Enemy.h"

void Enemy::Start()
{
	transform = GetTransform();
	collider = GetComponent<Physics2D::CircleCollider>();
	material = GetGameObject()->GetChild(0)->GetComponent<Material>();
}

void Enemy::FixedUpdate()
{
	if (hitTimer > 0)
	{
		if (hitRed)
			material->color = Color{ 1,1,1,1 };
		else
			material->color = Color{ 1,0,0,1 };
		hitRed = !hitRed;
	}
	else
	{
		material->color = Color{ 1,1,1,1 };
	}
}

void Enemy::Update()
{
	deltaTime = Utility::UTime::Get()->DeltaTimeF();
	velocity = velocity + gravity * deltaTime;
	
	if (hitTimer > 0) hitTimer -= deltaTime;
}

void Enemy::LateUpdate()
{
	Vector2 hitVelocity = hitDirection * deltaTime;
	hitVelocity.x *= hitPower.x;
	hitVelocity.y *= hitPower.y;
	hitPower = hitPower * 0.9f;
	//Console::WriteLine(to_string(hitVelocity.x) + ", " + to_string(hitVelocity.y));

	transform->localPosition.value = transform->localPosition.value + velocity + hitVelocity;
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

void Enemy::GetHit(Vector2 from)
{
	hitDirection = (transform->localPosition.value - from).Normalize();
	hitDirection.y = 0.5f;
	hitDirection = hitDirection.Normalize();
	hitPower.x = 300.0f;
	hitPower.y = 300.0f;
	hitTimer = hitMaxTimer;
	material->color = Color{ 1,1,1,1 };
	hitRed = true;
}


