#include "pch.h"
#include "Player.h"


void Player::Start()
{
	transform = GetTransform();
	imageObj = FindObjectWithName("playerImage");
	material = imageObj->GetComponent<Material>();

	player_animation_map[PLAYER_ANIMATION_STATE_IDLE] = Animation(true, 0.2f, {0, 0}, { 13, 0 }, { frameSize, frameSize }, { 416.0f, 416.0f });
	player_animation_map[PLAYER_ANIMATION_STATE_MOVE] = Animation(true, 0.1f, {0, 1}, { 8, 1 }, { frameSize, frameSize }, { 416.0f, 416.0f });
	player_animation_map[PLAYER_ANIMATION_STATE_ATTACK1] = Animation(false, 0.1f, { 0, 2 }, { 10, 2 }, { frameSize, frameSize }, { 416.0f, 416.0f });
	player_animation_map[PLAYER_ANIMATION_STATE_ATTACK2] = Animation(false, 0.1f, { 0, 3 }, { 10, 3 }, { frameSize, frameSize }, { 416.0f, 416.0f });
	player_animation_map[PLAYER_ANIMATION_STATE_ATTACK3] = Animation(false, 0.1f, { 0, 4 }, { 10, 4 }, { frameSize, frameSize }, { 416.0f, 416.0f });
	player_animation_map[PLAYER_ANIMATION_STATE_JUMP] = Animation(false, 0.1f, { 2, 5 }, { 6, 5 }, { frameSize, frameSize }, { 416.0f, 416.0f });
	player_animation_map[PLAYER_ANIMATION_STATE_DAMAGED] = Animation(false, 0.2f, { 0, 6 }, { 4, 6 }, { frameSize, frameSize }, { 416.0f, 416.0f });
	player_animation_map[PLAYER_ANIMATION_STATE_DEAD] = Animation(false, 0.2f, { 0, 7 }, { 7, 7 }, { frameSize, frameSize }, { 416.0f, 416.0f });
	player_animation_map[PLAYER_ANIMATION_STATE_CLIMB] = Animation(true, 0.2f, { 0, 8 }, { 4, 8 }, { frameSize, frameSize }, { 416.0f, 416.0f });
	player_animation_map[PLAYER_ANIMATION_STATE_ARROW_ATTACK] = Animation(false, 0.2f, { 0, 9 }, { 8, 9 }, { frameSize, frameSize }, { 416.0f, 416.0f });
	player_animation_map[PLAYER_ANIMATION_STATE_MAGIC] = Animation(false, 0.2f, { 0, 10 }, { 6, 10 }, { frameSize, frameSize }, { 416.0f, 416.0f });
	player_animation_map[PLAYER_ANIMATION_STATE_UP_RUNNING] = Animation(true, 0.2f, { 0, 11 }, { 8, 11 }, { frameSize, frameSize }, { 416.0f, 416.0f });
	player_animation_map[PLAYER_ANIMATION_STATE_ROLL] = Animation(false, 0.2f, { 0, 12 }, { 5, 12 }, { frameSize, frameSize }, { 416.0f, 416.0f });

	animation = player_animation_map[PLAYER_ANIMATION_STATE_IDLE];
	animation.Play();
}

void Player::Update()
{
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
		{
			animation.Change(player_animation_map[PLAYER_ANIMATION_STATE_IDLE]);
		}
		else
		{
			animation.Change(player_animation_map[PLAYER_ANIMATION_STATE_MOVE]);
		}
	}
}

void Player::LateUpdate()
{
	transform->localPosition.value = transform->localPosition.value + velocity + externalVelocity;
	velocity = Vector2(0, 0);
	if (Rotatable)
	{
		auto mousePos = Math::GetMousePosToWorld();
		auto mouseDirection = mousePos - transform->localPosition.value;
		RotateOn(mouseDirection.x);
	}
	material->uv = animation.Update();
	lastPos = transform->localPosition.value;
	timer += deltaTime;
}

void Player::RotateOn(float x)
{
	if (x < 0) imageObj->GetTransform()->localRotation.value.y = -Utility::UMath::PI;
	else if (x > 0) imageObj->GetTransform()->localRotation.value.y = 0;
}

void Player::ReceiveInput()
{
	if (Movable)
	{
		if (UEngine::Input::GetKey('S'))
		{
			auto value = Vector2(0, -1) * frameSize * speed * deltaTime;
			velocity = velocity + value;
		}
		if (UEngine::Input::GetKey('W'))
		{
			auto value = Vector2(0, 1) * frameSize * speed * deltaTime;
			velocity = velocity + value;
		}
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

	if (Input::GetMouseDown(VK_LBUTTON))
	{
		// TODO: IMPLEMENT JUMP COMBO ATTACK
		Movable = false;
		if (animation == player_animation_map[PLAYER_ANIMATION_STATE_ATTACK3])
			animation.Change(player_animation_map[PLAYER_ANIMATION_STATE_ATTACK2]);
		else if (animation == player_animation_map[PLAYER_ANIMATION_STATE_ATTACK2])
			animation.Change(player_animation_map[PLAYER_ANIMATION_STATE_ATTACK1]);
		else
			animation.Change(player_animation_map[PLAYER_ANIMATION_STATE_ATTACK3]);
	}

	if (animation != player_animation_map[PLAYER_ANIMATION_STATE_JUMP] &&
		Input::GetMouseDown(VK_RBUTTON))
	{
		EnableRoutine = false;
		jumpPower = 600.0f;
		animation.Change(player_animation_map[PLAYER_ANIMATION_STATE_JUMP]);
		return;
	}

	if (animation != player_animation_map[PLAYER_ANIMATION_STATE_ROLL] &&
		Input::GetMouseDown(VK_SPACE))
	{
		Rotatable = false;
		Movable = false;
		animation.Change(player_animation_map[PLAYER_ANIMATION_STATE_ROLL]);
		RotateOn(velocity.x);
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
		if (animation.IsFinished() && velocity.y == 0) animation.Change(player_animation_map[PLAYER_ANIMATION_STATE_IDLE]);
	}
	else if (animation == player_animation_map[PLAYER_ANIMATION_STATE_ATTACK1] ||
		animation == player_animation_map[PLAYER_ANIMATION_STATE_ATTACK2] ||
		animation == player_animation_map[PLAYER_ANIMATION_STATE_ATTACK3])
	{
		Movable = false;
		if (animation.IsFinished()) animation.Change(player_animation_map[PLAYER_ANIMATION_STATE_IDLE]);
	}
	else if (animation == player_animation_map[PLAYER_ANIMATION_STATE_ROLL])
	{
		Rotatable = false;
		Movable = false;
		if (animation.IsFinished()) animation.Change(player_animation_map[PLAYER_ANIMATION_STATE_IDLE]);
	}
}

void Player::OnPreRender()
{
	GetGameObject()->GetScene()->partition2D->DebugRender(GetGameObject()->GetScene()->partition2D->head, GetComponent<Physics2D::CircleCollider>(), Color{ 1, 0, 0, 1 }, Color{ 0, 0, 1, 1 });
	GetGameObject()->GetScene()->partition2D->DebugRender(GetGameObject()->GetScene()->partition2D->head, GetComponent<Physics2D::CircleCollider>(), Color{ 1, 1, 0, 1 });
}

void Player::OnCollisionStay(Physics2D::Collider* collisions)
{
	if (collisions->GetGameObject()->name == "tile")
	{
		externalVelocity.y = 0;
	}
}
