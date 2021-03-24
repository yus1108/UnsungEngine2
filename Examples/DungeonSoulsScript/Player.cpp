#include "pch.h"
#include "Attack.h"
#include "Player.h"


void Player::Start()
{
	transform = GetTransform();
	bodyObj = GetGameObject()->GetChild(0);
	material = bodyObj->GetComponent<Material>();
	health = GetGameObject()->GetChild(1)->GetComponent<Health>();
	weapon = GetGameObject()->GetChild(2)->GetComponent<Weapon>();

	bodyObj->GetTransform()->localPosition.value.x = 7.0f;

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

void Player::FixedUpdate()
{
	if (health->Dead) return;

	if (hitTimer > 0)
	{
		IsHit = true;
		if (hitRed)
			material->color = Color{ 1,1,1,1 };
		else
			material->color = Color{ 1,0,0,1 };
		hitRed = !hitRed;
	}
	else
	{
		material->color = Color{ 1,1,1,1 };
		IsHit = false;
	}

	deltaTime = GameState::GetCurrentFixedTimestep();

	
	
	
}


void Player::Update()
{
	weapon->Reset();
	deltaTime = Utility::UTime::Get()->DeltaTimeF();

	

	velocity = Vector2(0, 0);
	gVelocity += gravity * deltaTime;
	weight.y += gVelocity * deltaTime;

	if (attackDashTimer > 0)
	{
		if (transform->localRotation.value.y < 0)
			attackDashValue = -attackDash * deltaTime;
		else
			attackDashValue = attackDash * deltaTime;
		attackDashTimer -= deltaTime;
	}
	else
		attackDashValue = 0;

	if (dashCooldownTimer > 0) dashCooldownTimer -= deltaTime;
	if (dashTimer > 0)
	{
		if (transform->localRotation.value.y < 0)
			dashValue = -dashPower * deltaTime;
		else
			dashValue = dashPower * deltaTime;
		dashTimer -= deltaTime;
	}
	else
		dashValue = 0;
	dashDisplacement.x = attackDashValue + dashValue;

	if (jumpTimer > 0)
	{
		DashUp(jumpPower);
		jumpTimer -= deltaTime;
	}
	else
		dashDisplacement.y = 0;

	if (health->Dead) return;

	if (hitTimer > 0) hitTimer -= deltaTime;

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
	if (health->Dead)
		animation.Change(player_animation_map[PLAYER_ANIMATION_STATE_DEAD]);

	Vector2 hitVelocity = hitDirection * deltaTime;
	hitVelocity.x *= hitPower.x;
	hitVelocity.y *= hitPower.y;
	hitPower = hitPower * 0.9f;
	transform->localPosition.value = transform->localPosition.value + velocity + weight + dashDisplacement + hitVelocity;
	Console::Clear();
	Console::WriteLine(string("framepersecond : ") + to_string(Utility::UTime::Get()->FramePerSecond()));
	Console::WriteLine(string("deltatime : ") + to_string(deltaTime));
	Console::WriteLine(string("Player : ") + to_string(health->GetHP()));
	Console::WriteLine(string("Player gVelocity : ") + to_string(gVelocity));
	Console::WriteLine(string("Player weight.y : ") + to_string(weight.y));
	Console::WriteLine(string("Player jumpable : ") + (Jumpable ? "true" : "false"));
	Console::WriteLine(string("Player dashDisplacement.x : ") + to_string(dashDisplacement.x));
	Console::WriteLine(string(" : ") + to_string(GameState::GetCurrentFixedTimestep()));

	//Console::WriteLine(string("Monster : ") + to_string(FindObjectWithName("enemyBody")->GetComponent<Health>()->GetHP()));

	material->uv = animation.Update();

	auto mousePos = Math::GetMousePosToWorld();
	if (creation != nullptr)
	{
		creation->GetTransform()->localPosition.value = mousePos;
	}

	if (health->Dead) return;

	if (Rotatable)
	{
		auto mouseDirection = mousePos - transform->localPosition.value;
		RotateOn(mouseDirection.x);
	}
}

void Player::RotateOn(float x)
{
	if (x < 0) transform->localRotation.value.y = -Utility::UMath::PI;
	else if (x > 0) transform->localRotation.value.y = 0;
}

void Player::ReceiveInput()
{
	if (!UEngine::Input::GetKeyDown(VK_LMENU) && UEngine::Input::GetKeyDown(VK_TAB))
	{
		EditorMode = !EditorMode;
	}
	

	if (UEngine::Input::GetKeyDown('C'))
	{
		showCollision = !showCollision;
	}

	if (Movable && !IsHit)
	{
		if (UEngine::Input::GetKey('D'))
		{
			float xValue = frameSize * speed * deltaTime;
			velocity.x = xValue;
		}
		if (UEngine::Input::GetKey('A'))
		{
			auto xValue = -1.0f * frameSize * speed * deltaTime;
			velocity.x = xValue;
		}
	}

	if (!EditorMode)
	{
		AttackInput();
	}
	else
	{
		if (Input::GetKeyDown('3'))
		{
			creation = GameObject::Instantiate("tile");
			creation->IsStatic = true;
			creation->AddComponent<RenderComponent>()->Load("rectangle", "sprite");
			creation->AddComponent<Material>()->LoadImageMaterial(L"./Assets/tileset.png");
			auto collider = creation->AddComponent<Physics2D::RectCollider>();
			collider->SetCollider(32, 32);
			collider->IsTrigger = true;
			creation->GetTransform()->localPosition.value.x = 0;
			creation->GetTransform()->localPosition.value.y = 0;
			creation->GetTransform()->scale.value = Vector2{
				32,
				32
			};
			creation->GetComponent<Material>()->uv = UV
			{
				7.0f / 12.0f,
				0.5f,
				8.0f / 12.0f,
				1.0f - 1.0f / 3.0f
			};
		}
		if (Input::GetKeyDown('4'))
		{
			GetGameObject()->GetScene()->RemoveGameObject(&creation);
		}
	}

	if (Jumpable &&
		animation != player_animation_map[PLAYER_ANIMATION_STATE_JUMP] &&
		Input::GetKeyDown(VK_SPACE))
	{
		EnableRoutine = false;
		Jumpable = false;
		jumpTimer = jumpDuration;
		animation.Change(player_animation_map[PLAYER_ANIMATION_STATE_JUMP]);
		return;
	}

	if (animation != player_animation_map[PLAYER_ANIMATION_STATE_ROLL] &&
		dashCooldownTimer <= 0 &&
		Input::GetMouseDown(VK_RBUTTON))
	{
		Rotatable = false;
		EnableRoutine = false;

		dashTimer = dashDuration;
		dashCooldownTimer = dashCooldown;
		dashAction = PLAYER_DASH_ACTION_ROLL;
		animation.Change(player_animation_map[PLAYER_ANIMATION_STATE_ROLL]);
		return;
	}
}

void Player::UpdateAnimation()
{
	if (animation == player_animation_map[PLAYER_ANIMATION_STATE_JUMP])
	{
		EnableRoutine = false;
		if (Jumpable) animation.Change(player_animation_map[PLAYER_ANIMATION_STATE_IDLE]);
	}
	else if (animation == player_animation_map[PLAYER_ANIMATION_STATE_ATTACK1] ||
		animation == player_animation_map[PLAYER_ANIMATION_STATE_ATTACK2] ||
		animation == player_animation_map[PLAYER_ANIMATION_STATE_ATTACK3])
	{
		Movable = false;
		
		weapon->Set();
		
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
		GetGameObject()->GetScene()->partition2D->DebugRender(GetGameObject()->GetScene()->partition2D->head, GetComponent<Physics2D::CircleCollider>(), Color{ 1, 0, 0, 1 }, Color{ 0, 0, 1, 1 });
}

void Player::OnCollisionEnter(Physics2D::Collider* collisions)
{
	if (collisions->GetGameObject()->name == "tile")
	{
		weight = Vector2();
		gVelocity = 0;
		if (!Jumpable &&
			collisions->GetTransform()->localPosition.value.y < transform->localPosition.value.y)
			Jumpable = true;
	}
}

void Player::OnCollisionStay(Physics2D::Collider* collisions)
{
	if (collisions->GetGameObject()->name == "tile")
	{
		if (Jumpable)
		{
			gVelocity = 0;
			weight = Vector2();
		}
	}
}

void Player::OnCollisionExit(Physics2D::Collider* collisions)
{

}

void Player::Dash(float value)
{
	if (transform->localRotation.value.y < 0)
		dashDisplacement.x = -value * deltaTime;
	else
		dashDisplacement.x = value * deltaTime;
}

void Player::DashUp(float value)
{
	dashDisplacement.y = value * deltaTime;
}

void Player::AttackInput()
{
	float attackValue = 0;
	if (Input::GetMouseDown(VK_LBUTTON))
	{
		if (animation == player_animation_map[PLAYER_ANIMATION_STATE_ATTACK1])
			attackValue = static_cast<Attack>(player_animation_map[PLAYER_ANIMATION_STATE_ATTACK1])
			.ReceiveInput(animation, player_animation_map[PLAYER_ANIMATION_STATE_ATTACK2], attackDash);
		else if (animation == player_animation_map[PLAYER_ANIMATION_STATE_ATTACK2])
			attackValue = static_cast<Attack>(player_animation_map[PLAYER_ANIMATION_STATE_ATTACK2])
			.ReceiveInput(animation, player_animation_map[PLAYER_ANIMATION_STATE_ATTACK3], attackDash);
		else if (animation == player_animation_map[PLAYER_ANIMATION_STATE_ATTACK3])
			attackValue = static_cast<Attack>(player_animation_map[PLAYER_ANIMATION_STATE_ATTACK3])
			.ReceiveInput(animation, player_animation_map[PLAYER_ANIMATION_STATE_ATTACK1], attackDash);
		else
		{
			attackValue = attackDash;
			animation.Change(player_animation_map[PLAYER_ANIMATION_STATE_ATTACK1]);
		}
	}
	

	if (attackValue > 0)
	{
		Movable = false;
		attackDashTimer = attackDashDuration;
		dashAction = PLAYER_DASH_ACTION_ATTACK;
		SetAttack();
	}
}

void Player::SetAttack()
{
	weapon->Attack();
}

void Player::GetHit(Vector2 from)
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

