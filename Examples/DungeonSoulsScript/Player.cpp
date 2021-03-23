#include "pch.h"
#include "Attack.h"
#include "Player.h"


void Player::Start()
{
	transform = GetTransform();
	bodyObj = FindObjectWithName("playerBody");
	bodyObj->GetTransform()->localPosition.value.x = 7.0f;
	material = bodyObj->GetComponent<Material>();
	weapon = GetComponent<Weapon>();

	player_animation_map[PLAYER_ANIMATION_STATE_IDLE] = Animation(true, 0.2f, { 0, 0 }, { 13, 0 }, { frameSize, frameSize }, { 416.0f, 416.0f }, { 5.0f, 0 });
	player_animation_map[PLAYER_ANIMATION_STATE_MOVE] = Animation(true, 0.1f, {0, 1}, { 8, 1 }, { frameSize, frameSize }, { 416.0f, 416.0f }, { 5.0f, 0 });
	player_animation_map[PLAYER_ANIMATION_STATE_ATTACK1] = Attack(4); 
	player_animation_map[PLAYER_ANIMATION_STATE_ATTACK2] = Attack(3);
	player_animation_map[PLAYER_ANIMATION_STATE_ATTACK3] = Attack(2);
	player_animation_map[PLAYER_ANIMATION_STATE_JUMP] = Animation(false, 0.1f, { 2, 5 }, { 5, 5 }, { frameSize, frameSize }, { 416.0f, 416.0f }, { 5.0f, 0 });
	player_animation_map[PLAYER_ANIMATION_STATE_DAMAGED] = Animation(false, 0.2f, { 0, 6 }, { 4, 6 }, { frameSize, frameSize }, { 416.0f, 416.0f }, { 5.0f, 0 });
	player_animation_map[PLAYER_ANIMATION_STATE_DEAD] = Animation(false, 0.2f, { 0, 7 }, { 7, 7 }, { frameSize, frameSize }, { 416.0f, 416.0f }, { 5.0f, 0 });
	player_animation_map[PLAYER_ANIMATION_STATE_CLIMB] = Animation(true, 0.2f, { 0, 8 }, { 4, 8 }, { frameSize, frameSize }, { 416.0f, 416.0f }, { 5.0f, 0 });
	player_animation_map[PLAYER_ANIMATION_STATE_ARROW_ATTACK] = Animation(false, 0.2f, { 0, 9 }, { 8, 9 }, { frameSize, frameSize }, { 416.0f, 416.0f }, { 5.0f, 0 });
	player_animation_map[PLAYER_ANIMATION_STATE_MAGIC] = Animation(false, 0.2f, { 0, 10 }, { 6, 10 }, { frameSize, frameSize }, { 416.0f, 416.0f }, { 5.0f, 0 });
	player_animation_map[PLAYER_ANIMATION_STATE_UP_RUNNING] = Animation(true, 0.2f, { 0, 11 }, { 8, 11 }, { frameSize, frameSize }, { 416.0f, 416.0f }, { 5.0f, 0 });
	player_animation_map[PLAYER_ANIMATION_STATE_ROLL] = Animation(false, 0.1f, { 0, 12 }, { 5, 12 }, { frameSize, frameSize }, { 416.0f, 416.0f }, { 5.0f, 0 });

	animation = player_animation_map[PLAYER_ANIMATION_STATE_IDLE];
	animation.Play();
}

void Player::Update()
{
	weapon->Reset();
	deltaTime = Utility::UTime::Get()->DeltaTimeF();
	externalVelocity = externalVelocity + gravity * deltaTime;

	ReceiveInput();

	Rotatable = true;
	Movable = true;
	EnableRoutine = true;
	UpdateAnimation();

	if (Movable && EnableRoutine)
	{
		if (velocity.x == 0 && velocity.y == 0)
			animation.Change(player_animation_map[PLAYER_ANIMATION_STATE_IDLE]);
		else
			animation.Change(player_animation_map[PLAYER_ANIMATION_STATE_MOVE]);
	}
}

void Player::LateUpdate()
{
	externalVelocity.x += dashPower * deltaTime;
	DecreaseDash(600.0f);
	transform->localPosition.value = transform->localPosition.value + velocity + externalVelocity;
	velocity = Vector2(0, 0);
	if (Rotatable)
	{
		auto mousePos = Math::GetMousePosToWorld();
		auto mouseDirection = mousePos - transform->localPosition.value;
		RotateOn(mouseDirection.x);
	}
	material->uv = animation.Update();

	if (jump)
	{
		jumpCooldownTimer += deltaTime;
		if (jumpCooldownTimer > jumpCooldown)
		{
			jump = false;
			jumpCooldownTimer = 0;
		}
	}
	if (dash)
	{
		dashCooldownTimer += deltaTime;
		if (dashCooldownTimer > dashCooldown)
		{
			dash = false;
			dashCooldownTimer = 0;
		}
	}
}

void Player::RotateOn(float x)
{
	if (x < 0) transform->localRotation.value.y = -Utility::UMath::PI;
	else if (x > 0) transform->localRotation.value.y = 0;
}

void Player::ReceiveInput()
{
	if (UEngine::Input::GetKeyDown('C'))
	{
		showCollision = !showCollision;
	}

	if (Movable)
	{
		if (UEngine::Input::GetKey('D'))
		{
			auto value = Vector2(1, 0) * frameSize * speed * deltaTime;
			velocity = velocity + value;
		}
		if (UEngine::Input::GetKey('A'))
		{
			auto value = Vector2(-1, 0) * frameSize * speed * deltaTime;
			velocity = velocity + value;
		}
	}

	AttackInput();

	if (Jumpable && !jump &&
		animation != player_animation_map[PLAYER_ANIMATION_STATE_JUMP] &&
		Input::GetKeyDown(VK_SPACE))
	{
		EnableRoutine = false;
		Jumpable = false;
		jump = true;
		jumpPower = 600.0f;
		animation.Change(player_animation_map[PLAYER_ANIMATION_STATE_JUMP]);
		return;
	}

	if (!dash && animation != player_animation_map[PLAYER_ANIMATION_STATE_ROLL] &&
		Input::GetMouseDown(VK_RBUTTON))
	{
		Rotatable = false;
		EnableRoutine = false;
		dash = true;

		animation.Change(player_animation_map[PLAYER_ANIMATION_STATE_ROLL]);
		Dash(80.0f);
		return;
	}
}

void Player::UpdateAnimation()
{
	if (animation == player_animation_map[PLAYER_ANIMATION_STATE_JUMP])
	{
		EnableRoutine = false;
		externalVelocity.y = jumpPower * deltaTime;
		jumpPower -= 1200.0f * deltaTime;
		if (jumpPower < 0)
			jumpPower = 0;
		if (animation.IsFinished() && velocity.y == 0) 
			animation.Change(player_animation_map[PLAYER_ANIMATION_STATE_IDLE]);
	}
	else if (animation == player_animation_map[PLAYER_ANIMATION_STATE_ATTACK1] ||
		animation == player_animation_map[PLAYER_ANIMATION_STATE_ATTACK2] ||
		animation == player_animation_map[PLAYER_ANIMATION_STATE_ATTACK3])
	{
		Movable = false;
		weapon->Set();
		if (animation.IsAt({ 3, PLAYER_ANIMATION_STATE_ATTACK1 })) SetAttack();
		else if (animation.IsAt({ 3, PLAYER_ANIMATION_STATE_ATTACK2 })) SetAttack();
		else if (animation.IsAt({ 3, PLAYER_ANIMATION_STATE_ATTACK3 })) SetAttack();
		
		if (animation.IsFinished()) animation.Change(player_animation_map[PLAYER_ANIMATION_STATE_IDLE]);
	}
	else if (animation == player_animation_map[PLAYER_ANIMATION_STATE_ROLL])
	{
		Rotatable = false;
		EnableRoutine = false;

		if (animation.IsFinished()) animation.Change(player_animation_map[PLAYER_ANIMATION_STATE_IDLE]);
	}
}

void Player::OnPreRender()
{
	if (showCollision)
	{
		GetGameObject()->GetScene()->partition2D->DebugRender(GetGameObject()->GetScene()->partition2D->head, GetComponent<Physics2D::CircleCollider>(), Color{ 1, 0, 0, 1 }, Color{ 0, 0, 1, 1 });
	}
}

void Player::OnCollisionEnter(Physics2D::Collider* collisions)
{
	if (collisions->GetGameObject()->name == "tile")
	{
		if (!Jumpable && collisions->GetTransform()->localPosition.value.y < transform->localPosition.value.y)
			Jumpable = true;
	}
}

void Player::OnCollisionStay(Physics2D::Collider* collisions)
{
	if (collisions->GetGameObject()->name == "tile")
	{
		externalVelocity.y = 0;
	}
}

void Player::DecreaseDash(float value)
{
	if (transform->localRotation.value.y < 0)
	{
		dashPower += value * deltaTime;
		if (dashPower > 0)
		{
			dashPower = 0;
			externalVelocity.x = 0;
		}
	}
	// right
	else
	{
		dashPower -= value * deltaTime;
		if (dashPower < 0)
		{
			dashPower = 0;
			externalVelocity.x = 0;
		}
	}
}

void Player::Dash(float value)
{
	if (transform->localRotation.value.y < 0)
		dashPower = -value;
	else
		dashPower = value;
}

void Player::AttackInput()
{
	float attackValue = 0;
	if (Input::GetMouseDown(VK_LBUTTON))
	{
		if (animation == player_animation_map[PLAYER_ANIMATION_STATE_ATTACK1])
			attackValue = static_cast<Attack>(player_animation_map[PLAYER_ANIMATION_STATE_ATTACK1])
			.ReceiveInput(animation, player_animation_map[PLAYER_ANIMATION_STATE_ATTACK2], 60.0f);
		else if (animation == player_animation_map[PLAYER_ANIMATION_STATE_ATTACK2])
			attackValue = static_cast<Attack>(player_animation_map[PLAYER_ANIMATION_STATE_ATTACK2])
			.ReceiveInput(animation, player_animation_map[PLAYER_ANIMATION_STATE_ATTACK3], 60.0f);
		else if (animation == player_animation_map[PLAYER_ANIMATION_STATE_ATTACK3])
			attackValue = static_cast<Attack>(player_animation_map[PLAYER_ANIMATION_STATE_ATTACK3])
			.ReceiveInput(animation, player_animation_map[PLAYER_ANIMATION_STATE_ATTACK1], 60.0f);
		else
		{
			attackValue = 60.0f;
			animation.Change(player_animation_map[PLAYER_ANIMATION_STATE_ATTACK1]);
		}
	}
	

	if (attackValue > 0)
	{
		weapon->Set();
		Movable = false;
		Dash(attackValue);
	}
}

void Player::SetAttack()
{
	weapon->Attack();
}
