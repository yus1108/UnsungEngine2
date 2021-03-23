#include "pch.h"
#include "Enemy.h"

void Enemy::Start()
{
	transform = GetTransform();
	collider = GetComponent<Physics2D::CircleCollider>();
	material = GetGameObject()->GetChild(0)->GetComponent<Material>();
	health = GetGameObject()->GetChild(0)->GetComponent<Health>();
	weapon = GetGameObject()->GetChild(1)->GetComponent<Weapon>();

	player = FindObjectWithName("player")->GetComponent<Player>();
	playerCollider = player->GetComponent<Physics2D::CircleCollider>();

	attackTimer = AttackTime;
}

void Enemy::FixedUpdate()
{
	if (health->Dead) return;
	if (player->EditorMode) return;

	sight = Physics2D::MakeAABB(localSight, transform->GetWorld());
	range = Physics2D::MakeAABB(localRange, transform->GetWorld());
	attackable = false;
	weapon->Reset();
	if (Math::Physics2D::IsColliding(sight, playerCollider->GetCollider()))
	{
		playerFoundTimer = PlayerFoundDuration;
		if (Math::Physics2D::IsColliding(range, playerCollider->GetCollider()))
		{
			attackable = true;
			if (attackTimer <= 0)
			{
				weapon->Set();
				attackTimer = AttackTime;
			}
		}
	}

	if (hitTimer > 0)
	{
		movable = false;
		if (hitRed)
			material->color = Color{ 1,1,1,1 };
		else
			material->color = Color{ 1,0,0,1 };
		hitRed = !hitRed;
	}
	else
	{
		material->color = Color{ 1,1,1,1 };
		movable = true;
	}
}

void Enemy::Update()
{
	if (player->EditorMode) return;

	deltaTime = Utility::UTime::Get()->DeltaTimeF();
	velocity = velocity + gravity * deltaTime;

	if (health->Dead) return;

	if (attackCooldown > 0) attackCooldown -= deltaTime;
	if (hitTimer > 0) hitTimer -= deltaTime;
	if (attackable && attackTimer > 0) attackTimer -= deltaTime;

	if (movable)
	{
		if (playerFoundTimer > 0)
		{
			if (attackCooldown <= 0)
			{
				if (attackable)
				{
					attackCooldown = AttackDelay;
					weapon->Attack();
				}
				else
				{
					Vector2 moveVelocity =
						Vector2(
							playerCollider->GetCollider().center.x -
							transform->localPosition.value.x, 0
						).Normalize() * MoveSpeed * deltaTime;
					velocity = velocity + moveVelocity;
					RotateOn(moveVelocity.x);
					playerFoundTimer -= deltaTime;
				}
			}
			
		}
	}
}

void Enemy::LateUpdate()
{
	if (player->EditorMode) return;

	Vector2 hitVelocity = hitDirection * deltaTime;
	hitVelocity.x *= hitPower.x;
	hitVelocity.y *= hitPower.y;
	hitPower = hitPower * 0.9f;

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
	hitDirection = (GetTransform()->localPosition.value - from).Normalize();
	hitDirection.y = 0.5f;
	hitDirection = hitDirection.Normalize();
	hitPower.x = 500.0f;
	hitPower.y = 300.0f;
	hitTimer = hitMaxTimer;
	material->color = Color{ 1,1,1,1 };
	hitRed = true;
}

void Enemy::OnPreRender()
{
	if (FindObjectWithName("player")->GetComponent<Player>()->showCollision)
	{
		GetGameObject()->GetScene()->debugRenderer->Add_Rectangle(sight, Color{ 1, 1, 0, 1 });
		GetGameObject()->GetScene()->debugRenderer->Add_Rectangle(range, Color{ 0, 1, 0, 1 });
	}
}

void Enemy::RotateOn(float x)
{
	if (x < 0) transform->localRotation.value.y = -Utility::UMath::PI;
	else if (x > 0) transform->localRotation.value.y = 0;
}