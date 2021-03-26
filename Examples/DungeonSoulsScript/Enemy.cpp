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

	hitDashX.Update(deltaTime, false);
	hitDashY.Update(deltaTime, false);

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
					SoundManager::Get()->PlayAudio(4, L"Assets/sword sound.wav", AudioType_Effects);
				}
				else
				{
					if (velocity.y >= 0)
					{
						Vector2 moveVelocity =
							Vector2(
								playerCollider->GetCollider().center.x -
								transform->localPosition.value.x, 0
							).Normalize() * MoveSpeed * deltaTime;
						velocity = velocity + moveVelocity;
						RotateOn(moveVelocity.x);
					}
					playerFoundTimer -= deltaTime;
				}
			}
			
		}
		else if (idleWanderingTimer > WanderingWait)
		{
			if (velocity.y >= 0)
			{
				Vector2 moveVelocity =
					Vector2(wanderingDir, 0).Normalize() * MoveSpeed * deltaTime;
				velocity = velocity + moveVelocity;
				RotateOn(moveVelocity.x);
			}
			idleWanderingTimer -= deltaTime;
		}
		else if (idleWanderingTimer > 0)
		{
			idleWanderingTimer	-= deltaTime;
		}
		else
		{
			wanderingDir = Math::RndFloat(-1.0f, 1.0f);
			idleWanderingTimer = WanderingDuration;
		}
	}

	velocity = velocity + gravity * deltaTime;

}

void Enemy::LateUpdate()
{
	if (player->EditorMode) return;

	Vector2 hitVelocity = hitDirection;
	hitVelocity.x *= hitDashX.GetValue();
	hitVelocity.y *= hitDashY.GetValue();

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
	hitDashX.Activate();
	hitDashY.Activate();
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