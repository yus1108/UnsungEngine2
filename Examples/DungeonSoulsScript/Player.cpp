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
	SoundManager::Get()->InitAudio();
	SoundManager::Get()->PlayAudio(0, L"Assets/Tipple Chipper.wav", AudioType_Music);

	GameState::Get()->MaxFixedTimestep = 0.02f;
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
	DashUpdate();

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

	transform->localPosition.value = 
		transform->localPosition.value + 
		velocity + weight + 
		dashDisplacement + hitDashDisplacement;

	if (transform->localPosition.value.y > 360.0f)
		transform->localPosition.value.y = 360.0f;
	else if (transform->localPosition.value.y < -337.0f)
		transform->localPosition.value.y = -337.0f;
	if (transform->localPosition.value.x > 630.0f)
		transform->localPosition.value.x = 630.0f;
	else if (transform->localPosition.value.x < -630.0f)
		transform->localPosition.value.x = -630.0f;
	//Console::Clear();
	//Console::WriteLine(string("framepersecond : ") + to_string(Utility::UTime::Get()->FramePerSecond()));
	//Console::WriteLine(string("deltatime : ") + to_string(deltaTime));
	//Console::WriteLine(string("Player : ") + to_string(health->GetHP()));

	material->uv = animation.Update();

	auto mousePos = Math::GetMousePosToWorld();
	if (creation != nullptr)
	{
		creation->GetTransform()->localPosition.value = mousePos;
		creation->GetTransform()->localPosition.value.x =
			static_cast<int>(roundf(creation->GetTransform()->localPosition.value.x)) / 32 * 32.0f + 
			(mousePos.x > 0 ? 16.0f : -16.0f);
		creation->GetTransform()->localPosition.value.y =
			static_cast<int>(roundf(creation->GetTransform()->localPosition.value.y)) / 32 * 32.0f + 
			(mousePos.y > 0 ? 16.0f : -16.0f);
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
	if (!UEngine::Input::GetKey(VK_LMENU) && UEngine::Input::GetKeyDown(VK_TAB))
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
		if (creation) GetGameObject()->GetScene()->RemoveGameObject(&creation);
		deletionMode = false;
		AttackInput();
	}
	else
	{
		if (Input::GetKeyDown('3'))
		{
			deletionMode = false;
			if (!creation)
			{
				CreateTile();
			}
			else
			{
				auto name = creation->name;
				GetGameObject()->GetScene()->RemoveGameObject(&creation);
				if (name == "enemy") CreateTile();
			}
			
		}
		else if (Input::GetKeyDown('4'))
		{
			deletionMode = false;
			if (!creation)
			{
				CreateSkeleton();
			}
			else
			{
				auto name = creation->name;
				GetGameObject()->GetScene()->RemoveGameObject(&creation);
				if (name == "tile") CreateSkeleton();
			}

		}
		if (Input::GetKeyDown(VK_DELETE))
		{
			GetGameObject()->GetScene()->RemoveGameObject(&creation);
			deletionMode = !deletionMode;
		}

		if (Input::GetMouseDown(VK_LBUTTON))
		{
			if (creation != nullptr)
			{
				auto objs = GetGameObject()->GetScene()->GetGameObjects();
				bool isTileExist = false;
				for (auto i = objs.begin(); i != objs.end(); i++)
				{
					if (*i == creation) continue;
					if ((*i)->name == "tile")
					{
						if (Math::Physics2D::IsColliding
						(
							creation->GetTransform()->localPosition.value,
							(*i)->GetComponent<Physics2D::RectCollider>()->GetCollider()
						))
						{
							isTileExist = true;
							break;
						}
					}
					else if ((*i)->name == "enemy")
					{
						if (Math::Physics2D::IsColliding
						(
							creation->GetTransform()->localPosition.value,
							(*i)->GetComponent<Physics2D::CircleCollider>()->GetCollider()
						))
						{
							isTileExist = true;
							break;
						}
					}
				}

				if (!isTileExist)
				{
					if ((creation->name == "tile"))
					{
						creation->GetComponent<Physics2D::RectCollider>()->IsTrigger = false;
						CreateTile();
					}
					if ((creation->name == "enemy"))
					{
						CreateSkeleton();
					}
				}
					
			}
			else if (deletionMode)
			{
				auto mousePos = Math::GetMousePosToWorld();
				auto objs = GetGameObject()->GetScene()->GetGameObjects();
				for (auto i = objs.begin(); i != objs.end(); i++)
				{
					if (*i == creation) continue;
					if ((*i)->name == "tile")
					{
						if (Math::Physics2D::IsColliding
						(
							mousePos,
							(*i)->GetComponent<Physics2D::RectCollider>()->GetCollider()
						))
						{
							GetGameObject()->GetScene()->RemoveGameObject(&(*i));
							break;
						}
					}
					else if ((*i)->name == "enemy")
					{
						if (Math::Physics2D::IsColliding
						(
							mousePos,
							(*i)->GetComponent<Physics2D::CircleCollider>()->GetCollider()
						))
						{
							GetGameObject()->GetScene()->RemoveGameObject(&(*i));
							break;
						}
					}
				}
			}
		}
	}

	if (Jumpable &&
		animation != player_animation_map[PLAYER_ANIMATION_STATE_JUMP] &&
		Input::GetKeyDown(VK_SPACE))
	{
		EnableRoutine = false;
		Jumpable = false;
		jumpDash.Activate();
		animation.Change(player_animation_map[PLAYER_ANIMATION_STATE_JUMP]);
		SoundManager::Get()->PlayAudio(3, L"Assets/Mario_Jumping-Mike_Koenig-989896458.wav", AudioType_Effects);
		return;
	}

	if (animation != player_animation_map[PLAYER_ANIMATION_STATE_ROLL] &&
		dash.IsAvailable() &&
		Input::GetMouseDown(VK_RBUTTON))
	{
		Rotatable = false;
		EnableRoutine = false;

		dash.Activate();
		animation.Change(player_animation_map[PLAYER_ANIMATION_STATE_ROLL]);
		SoundManager::Get()->PlayAudio(2, L"Assets/whoosh (phaser).wav", AudioType_Effects);
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
	if (creation)
	{
		if (creation->name == "tile")
		{
			auto collider = creation->GetComponent<Physics2D::RectCollider>();
			GetGameObject()->GetScene()->debugRenderer->Add_Rectangle(collider->GetCollider(), Color{ 0, 1, 1, 1 });
		}
		else if (creation->name == "enemy")
		{
			auto collider = creation->GetComponent<Physics2D::CircleCollider>();
			GetGameObject()->GetScene()->debugRenderer->Add_Circle(collider->GetCollider(), Color{ 0, 1, 1, 1 });
		}
	}
	else if (deletionMode)
	{
		auto mousePos = Math::GetMousePosToWorld();
		Math::Physics2D::AABB aabb;
		aabb.left = mousePos.x - 16.0f;
		aabb.bottom = mousePos.y - 16.0f;
		aabb.right = mousePos.x + 16.0f;
		aabb.top = mousePos.y + 16.0f;
		GetGameObject()->GetScene()->debugRenderer->Add_Rectangle(aabb, Color{ 0, 1, 1, 1 });
	}
}

void Player::OnCollisionEnter(Physics2D::Collider* collisions)
{
	if (collisions->GetGameObject()->name == "tile")
	{
		if (collisions->GetTransform()->localPosition.value.y < transform->localPosition.value.y)
		{
			if (!Jumpable)
			{
				Jumpable = true;
				Vector2 diff = collisions->GetTransform()->localPosition.value - transform->localPosition.value;
				if (abs(diff.x) < abs(diff.y))
				{
					gVelocity = 0;
					weight = Vector2();
				}
				transform->localPosition.value.y = collisions->GetTransform()->localPosition.value.y + 32.0f;
				return;
			}
			return;
		}
		else if (Math::Physics2D::IsColliding(transform->localPosition.value, static_cast<Physics2D::RectCollider*>(collisions)->GetCollider()))
		{
			transform->localPosition.value.y = collisions->GetTransform()->localPosition.value.y + 32.0f;
			gVelocity = 0;
			weight = Vector2();
		}
		velocity = Vector2();
		jumpDash.Stop();
	}
}

void Player::OnCollisionStay(Physics2D::Collider* collisions)
{
	if (collisions->GetGameObject()->name == "tile")
	{
		if (collisions->GetTransform()->localPosition.value.y < transform->localPosition.value.y)
		{
			Vector2 diff = collisions->GetTransform()->localPosition.value - transform->localPosition.value;
			if (abs(diff.x) < abs(diff.y))
			{
				gVelocity = 0;
				weight = Vector2();
			}
			return;
		}
		else if (Math::Physics2D::IsColliding(transform->localPosition.value, static_cast<Physics2D::RectCollider*>(collisions)->GetCollider()))
		{
			transform->localPosition.value.y = collisions->GetTransform()->localPosition.value.y + 32.0f;
			gVelocity = 0;
			weight = Vector2();
		}
		
		velocity = Vector2();
	}
}

void Player::DashUpdate()
{
	jumpDash.Update(deltaTime, false);
	if (transform->localRotation.value.y < 0)
	{
		attackDash.Update(deltaTime, true);
		dash.Update(deltaTime, true);
	}
	else
	{
		attackDash.Update(deltaTime, false);
		dash.Update(deltaTime, false);
	}
	hitDashX.Update(deltaTime, false);
	hitDashY.Update(deltaTime, false);

	hitDashDisplacement.x = hitDashX.GetValue() * hitDirection.x;
	hitDashDisplacement.y = hitDashY.GetValue() * hitDirection.y;

	dashDisplacement.x = attackDash.GetValue() + dash.GetValue();
	dashDisplacement.y = jumpDash.GetValue();
}

void Player::AttackInput()
{
	float attackValue = 0;
	if (Input::GetMouseDown(VK_LBUTTON))
	{
		if (animation == player_animation_map[PLAYER_ANIMATION_STATE_ATTACK1])
			attackValue = static_cast<Attack>(player_animation_map[PLAYER_ANIMATION_STATE_ATTACK1])
			.ReceiveInput(animation, player_animation_map[PLAYER_ANIMATION_STATE_ATTACK2], PLAYER_ANIMATION_STATE_ATTACK1);
		else if (animation == player_animation_map[PLAYER_ANIMATION_STATE_ATTACK2])
			attackValue = static_cast<Attack>(player_animation_map[PLAYER_ANIMATION_STATE_ATTACK2])
			.ReceiveInput(animation, player_animation_map[PLAYER_ANIMATION_STATE_ATTACK3], PLAYER_ANIMATION_STATE_ATTACK2);
		else if (animation == player_animation_map[PLAYER_ANIMATION_STATE_ATTACK3])
			attackValue = static_cast<Attack>(player_animation_map[PLAYER_ANIMATION_STATE_ATTACK3])
			.ReceiveInput(animation, player_animation_map[PLAYER_ANIMATION_STATE_ATTACK1], PLAYER_ANIMATION_STATE_ATTACK3);
		else
		{
			attackValue = 1;
			animation.Change(player_animation_map[PLAYER_ANIMATION_STATE_ATTACK1]);
		}
	}
	

	if (attackValue > 0)
	{
		Movable = false;
		attackDash.Activate();
		SoundManager::Get()->PlayAudio(1, L"Assets/sword sound.wav", AudioType_Effects);
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
	hitDashX.Activate();
	hitDashY.Activate();
	hitTimer = hitMaxTimer;
	material->color = Color{ 1,1,1,1 };
	hitRed = true;
}

void Player::OnDestroy()
{
	GetGameObject()->GetScene()->RemoveGameObject(&creation);
	SoundManager::Get()->StopAudio();
	SoundManager::Release();
}

void Player::CreateTile()
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

void Player::CreateSkeleton()
{
	creation = GameObject::Instantiate("enemy");
	auto enemyCollider = creation->AddComponent<UEngine::Physics2D::CircleCollider>();
	enemyCollider->SetCollider({ 0, 0 }, 16);
	enemyCollider->IsTrigger = true;
	creation->GetTransform()->localPosition.value.x = 50.0f;
	creation->GetTransform()->localPosition.value.y = 200.0f;
	creation->AddComponent<Skeleton>();

	auto enemyBody = GameObject::Instantiate("enemyBody");
	enemyBody->AddComponent<RenderComponent>()->Load("rectangle", "sprite");
	auto enemyBodyCollider = enemyBody->AddComponent<UEngine::Physics2D::RectCollider>();
	enemyBodyCollider->SetCollider(16, 32);
	enemyBodyCollider->IsTrigger = true;
	auto enemyMaterial = enemyBody->AddComponent<Material>();
	enemyMaterial->LoadImageMaterial(L"./Assets/SkeletalWarrior_Sprites.png");
	enemyMaterial->uv.value = UV{ 0, 0, 1.0f / 10.0f, 1.0f / 10.0f };
	enemyBody->GetTransform()->scale = Vector2(32, 32);
	enemyBody->SetParent(creation);
	enemyBody->AddComponent<Health>();

	auto enemyAttack = GameObject::Instantiate("enemyAttack");
	auto eAttack = enemyAttack->AddComponent<UEngine::Physics2D::RectCollider>();
	eAttack->SetCollider(16, 32);
	eAttack->IsTrigger = true;
	enemyAttack->GetTransform()->localPosition.value.x = 8;
	enemyAttack->AddComponent<Weapon>();
	enemyAttack->SetParent(creation);
}
