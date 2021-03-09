#include "stdafx.h"
#include "Player.h"

void Player::Start()
{
	material = GetComponent<Material>();
	lastpos = GetTransform()->localPosition;
	collider = GetComponent<Physics2D::RectCollider>();
}

void Player::FixedUpdate()
{
	if (WinInput::Get()->GetKeyDown(VK_SPACE) && ableToJump)
	{
		ableToJump = false;
		weight = 20;
	}
	if (collider->collisions.size() > 0)
	{
		ableToJump = true;
		if (weight < 0)
		{
			weight = 0;
		}
	}
	else
	{
		ableToJump = false;
		if (GetTransform()->localPosition.y < -150)
		{
			weight += gravity * 10;
		}
	}
	GetTransform()->localPosition.y += weight;
	weight += gravity;
	lastpos = GetTransform()->localPosition;
}

void Player::Update()
{
	Vector2 dir1 = GetTransform()->localPosition - GetComponent<Physics2D::RectCollider>()->GetLastPos();
	std::cout << dir1.x << std::endl;
	std::cout << dir1.y << std::endl;
	

	auto transform = GetTransform();
	if (WinInput::Get()->GetKey('1'))
	{
		GetTransform()->localPosition.x = -450;
		GetTransform()->localPosition.y = -100;
		weight = 0;
	}
	if (WinInput::Get()->GetKey(VK_DOWN))
	{
		auto value = Vector2(0, -1) * 100 * Utility::UTime::Get()->DeltaTimeF();
		transform->localPosition = transform->localPosition + value;
	}
	if (WinInput::Get()->GetKey(VK_UP))
	{
		auto value = Vector2(0, 1) * 100 * Utility::UTime::Get()->DeltaTimeF();
		transform->localPosition = transform->localPosition + value;
	}
	if (WinInput::Get()->GetKey(VK_RIGHT))
	{
		auto value = Vector2(1, 0) * 100 * Utility::UTime::Get()->DeltaTimeF();
		transform->localPosition = transform->localPosition + value;
	}
	if (WinInput::Get()->GetKey(VK_LEFT))
	{
		auto value = Vector2(-1, 0) * 100 * Utility::UTime::Get()->DeltaTimeF();
		transform->localPosition = transform->localPosition + value;
	}
	

	if (frame == static_cast<float>(maxSpriteFrame) / maxMapIndex.x)
		currMapIndex.x = (currMapIndex.x + 1) % maxMapIndex.x;

	if (frame == static_cast<float>(maxSpriteFrame) / maxMapIndex.y)
		currMapIndex.y = (currMapIndex.y + 1) % maxMapIndex.y;

	material->uv = UV
	{
		static_cast<float>(currMapIndex.x) / maxMapIndex.x,
		static_cast<float>(currMapIndex.y) / maxMapIndex.y,
		static_cast<float>(currMapIndex.x + 0.9f) / maxMapIndex.x,
		static_cast<float>(currMapIndex.y + 1) / maxMapIndex.y,
	};

	if (frame == maxSpriteFrame)
		frame = 0;
	frame++;
}

void Player::OnPreRender()
{
	GameState* gameState = GameState::Get();
	auto sp = gameState->GetSpatialPartition2D();
	sp->DebugRender(sp->head, GetComponent<Physics2D::RectCollider>(), Color{ 1, 0, 0, 1 }, Color{ 0, 0, 1, 1 });
	sp->DebugRender(sp->head, GetComponent<Physics2D::RectCollider>(), Color{ 1, 1, 0, 1 });
}