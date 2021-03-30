#include "pch.h"
#include "Boss.h"

void Boss::BossUpdate()
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
					projectile = nullptr;
					attackType = Math::RndInt(0, 3);
					//attackType = 2;
					if (attackType == 0)
					{
						attackCooldown = AttackDelay;
						weapon->Attack();
						localRange = { -4, 32, 4, -32 };
						SoundManager::Get()->PlayAudio(4, L"Assets/sword sound.wav", AudioType_Effects);
					}
					else if (attackType == 1)
					{
						localRange = localSight;
						attackCooldown = 2.0f;

					}
					else if (attackType == 2)
					{
						localRange = localSight;
						attackCooldown = 3.0f;
					}
					else if (attackType == 3)
					{
						attackCooldown = 4.0f;
						localRange = { -32, 32, 32, -32 };
					}
				}
				else
				{
					projectile = nullptr;
					if (moveTimer > 0)
					{
						moveTimer -= deltaTime;
					}
					else
					{
						attackType = Math::RndInt(0, 3);
						//attackType = 2;
						moveTimer = moveDuration;
					}
					if (attackType == 0)
					{
						localRange = { -4, 32, 4, -32 };
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
					else if (attackType == 1)
					{
						localRange = localSight;
						attackCooldown = 2.0f;
						moveTimer = 0;
					}
					else if (attackType == 2)
					{
						localRange = localSight;
						attackCooldown = 3.0f;
						moveTimer = 0;
					}
					else if (attackType == 3)
					{
						localRange = { -32, 32, 32, -32 };
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
			else
			{
				Vector2 moveVelocity =
					Vector2(
						playerCollider->GetCollider().center.x -
						transform->localPosition.value.x, 0
					).Normalize() * MoveSpeed * deltaTime;
				RotateOn(moveVelocity.x);
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
			idleWanderingTimer -= deltaTime;
		}
		else
		{
			wanderingDir = Math::RndFloat(-1.0f, 1.0f);
			idleWanderingTimer = WanderingDuration;
		}
	}

	velocity = velocity + gravity * deltaTime;
}

void Boss::FixedUpdate()
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
				if (animation == boss_animation_map[BOSS_ANIMATION_STATE_ATTACK1] ||
					animation == boss_animation_map[BOSS_ANIMATION_STATE_ATTACK4])
				{
					weapon->Set();
					attackTimer = AttackTime;
				}
				else if (animation == boss_animation_map[BOSS_ANIMATION_STATE_ATTACK2])
				{
					CreateShooter();
					attackTimer = AttackTime;
				}
				else if (animation == boss_animation_map[BOSS_ANIMATION_STATE_ATTACK3])
				{
					CreateBarrel();
					attackTimer = AttackTime;
				}
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

void Boss::Start()
{
	Enemy::Start();

	boss_animation_map[BOSS_ANIMATION_STATE_IDLE] = Animation(true, 0.2f, { 0, BOSS_ANIMATION_STATE_IDLE }, { 5, BOSS_ANIMATION_STATE_IDLE }, { 96.0f, 96.0f }, { 1024.0f, 1920.0f }, { 0, 0 });
	boss_animation_map[BOSS_ANIMATION_STATE_MOVE] = Animation(true, 0.1f, { 0, BOSS_ANIMATION_STATE_MOVE }, { 8, BOSS_ANIMATION_STATE_MOVE }, { 96.0f, 96.0f }, { 1024.0f, 1920.0f }, { 0, 0 });
	boss_animation_map[BOSS_ANIMATION_STATE_ANGRY] = Animation(true, 0.1f, { 0, BOSS_ANIMATION_STATE_ANGRY }, { 5, BOSS_ANIMATION_STATE_ANGRY }, { 96.0f, 96.0f }, { 1024.0f, 1920.0f }, { 0, 0 });
	boss_animation_map[BOSS_ANIMATION_STATE_ATTACK1] = Animation(false, 0.1f, { 0, BOSS_ANIMATION_STATE_ATTACK1 }, { 9, BOSS_ANIMATION_STATE_ATTACK1 }, { 96.0f, 96.0f }, { 1024.0f, 1920.0f }, { 0, 0 });
	boss_animation_map[BOSS_ANIMATION_STATE_ATTACK2] = Animation(false, 0.1f, { 0, BOSS_ANIMATION_STATE_ATTACK2 }, { 5, BOSS_ANIMATION_STATE_ATTACK2 }, { 96.0f, 96.0f }, { 1024.0f, 1920.0f }, { 0, 0 });
	boss_animation_map[BOSS_ANIMATION_STATE_ATTACK3] = Animation(false, 0.1f, { 0, BOSS_ANIMATION_STATE_ATTACK3 }, { 6, BOSS_ANIMATION_STATE_ATTACK3 }, { 96.0f, 96.0f }, { 1024.0f, 1920.0f }, { 0, 0 });
	boss_animation_map[BOSS_ANIMATION_STATE_ATTACK4] = Animation(false, 0.1f, { 0, BOSS_ANIMATION_STATE_ATTACK4 }, { 9, BOSS_ANIMATION_STATE_ATTACK4 }, { 96.0f, 96.0f }, { 1024.0f, 1920.0f }, { 0, 0 });
	boss_animation_map[BOSS_ANIMATION_STATE_DAMAGED] = Animation(false, 0.2f, { 0, BOSS_ANIMATION_STATE_DAMAGED }, { 3, BOSS_ANIMATION_STATE_DAMAGED }, { 96.0f, 96.0f }, { 1024.0f, 1920.0f }, { 0, 0 });
	boss_animation_map[BOSS_ANIMATION_STATE_IDLE2] = Animation(false, 0.2f, { 0, BOSS_ANIMATION_STATE_IDLE2 }, { 3, BOSS_ANIMATION_STATE_IDLE2 }, { 96.0f, 96.0f }, { 1024.0f, 1920.0f }, { 0, 0 });
	boss_animation_map[BOSS_ANIMATION_STATE_DEAD] = Animation(false, 0.2f, { 0, BOSS_ANIMATION_STATE_DEAD }, { 6, BOSS_ANIMATION_STATE_DEAD }, { 96.0f, 96.0f }, { 1024.0f, 1920.0f }, { 0, 0 });

	animation = boss_animation_map[BOSS_ANIMATION_STATE_IDLE];
	animation.Play();

	localSight = { -384, 384, 384, -32 };
	localRange = { -4, 32, 4, -32 };
}

void Boss::Update()
{
	if (player == nullptr) return;
	if (player->EditorMode) return;

	BossUpdate();

	if (health->Dead)
	{
		animation.Change(boss_animation_map[BOSS_ANIMATION_STATE_DEAD]);
		return;
	}

	if (attackCooldown > 0)
	{
		if (attackType == 0)
		{
			animation.Change(boss_animation_map[BOSS_ANIMATION_STATE_ATTACK1]);
		}
		else if (attackType == 1)
		{
			if (attackCooldown < 0.6f)
			{
				animation.Change(boss_animation_map[BOSS_ANIMATION_STATE_ATTACK2]);
			}
			else
			{
				delayAttack = false;
				animation.Change(boss_animation_map[BOSS_ANIMATION_STATE_ANGRY]);
			}
		}
		else if (attackType == 2)
		{
			if (attackCooldown < 0.6f)
			{
				animation.Change(boss_animation_map[BOSS_ANIMATION_STATE_ATTACK3]);
			}
			else
			{
				delayAttack = false;
				animation.Change(boss_animation_map[BOSS_ANIMATION_STATE_ANGRY]);
			}
		}
		else if (attackType == 3)
		{
			if (attackCooldown < 1.0f)
			{
				if (delayAttack && animation != boss_animation_map[BOSS_ANIMATION_STATE_ANGRY])
				{
					weapon->Attack();
					SoundManager::Get()->PlayAudio(4, L"Assets/sword sound.wav", AudioType_Effects);
					delayAttack = false;
				}
				if (animation == boss_animation_map[BOSS_ANIMATION_STATE_ANGRY])
				{
					animation.Change(boss_animation_map[BOSS_ANIMATION_STATE_ATTACK4]);
					delayAttack = true;
				}
			}
			else
			{
				delayAttack = false;
				animation.Change(boss_animation_map[BOSS_ANIMATION_STATE_ANGRY]);
			}
		}
	}
	else
	{
		if (movable)
		{
			if (velocity.x == 0)
				animation.Change(boss_animation_map[BOSS_ANIMATION_STATE_IDLE]);
			else
				animation.Change(boss_animation_map[BOSS_ANIMATION_STATE_MOVE]);
		}
	}
}

void Boss::LateUpdate()
{
	if (player == nullptr) return;
	if (player->EditorMode) return;

	Enemy::LateUpdate();
	material->uv = animation.Update();
}

void Boss::GetHit(Vector2 from)
{
	hitTimer = hitMaxTimer;
	material->color = Color{ 1,1,1,1 };
	hitRed = true;
}

void Boss::CreateBarrel()
{
	if (projectile) return;

	projectile = GameObject::Instantiate("barrel");
	projectile->AddComponent<RenderComponent>()->Load("rectangle", "sprite");
	auto enemyMaterial = projectile->AddComponent<Material>();
	enemyMaterial->LoadImageMaterial(L"./Assets/Destructible Objects Sprite Sheet.png");
	enemyMaterial->uv.value = UV{ 0, 1.0f / 12.0f, 1.0f / 7.0f, 2.0f / 12.0f };
	auto projectileComponent = projectile->AddComponent<Projectile>();
	projectileComponent->velocity = Vector2(0, -200);
	projectileComponent->parent = health->GetGameObject();
	projectile->GetTransform()->scale = Vector2(32, 32);
	projectile->GetTransform()->localPosition.value = player->GetTransform()->localPosition.value;
	projectile->GetTransform()->localPosition.value.y += 100.0f;
	auto enemyCollider = projectile->AddComponent<UEngine::Physics2D::CircleCollider>();
	enemyCollider->SetCollider({ 0, 0 }, 16);
	enemyCollider->IsTrigger = true;
}

void Boss::CreateShooter()
{
	if (projectile) return;

	projectile = GameObject::Instantiate("barrel");
	projectile->AddComponent<RenderComponent>()->Load("rectangle", "sprite");
	auto enemyMaterial = projectile->AddComponent<Material>();
	enemyMaterial->LoadImageMaterial(L"./Assets/Destructible Objects Sprite Sheet.png");
	enemyMaterial->uv.value = UV{ 0, 1.0f / 12.0f, 1.0f / 7.0f, 2.0f / 12.0f };
	auto projectileComponent = projectile->AddComponent<Projectile>();
	float xDir = player->GetTransform()->localPosition.value.x - transform->localPosition.value.x;
	projectileComponent->velocity = Vector2(xDir, 0).Normalize() * 200.0f;
	projectileComponent->parent = health->GetGameObject();
	projectile->GetTransform()->scale = Vector2(32, 32);
	projectile->GetTransform()->localPosition.value = transform->localPosition.value;
	auto enemyCollider = projectile->AddComponent<UEngine::Physics2D::CircleCollider>();
	enemyCollider->SetCollider({ 0, 0 }, 16);
	enemyCollider->IsTrigger = true;
}