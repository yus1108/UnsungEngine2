#include "stdafx.h"
#include "Player.h"

void Player::Start()
{
	material = GetComponent<Material>();
	lastpos = GetTransform()->localPosition;
}

void Player::FixedUpdate()
{
	
}

void Player::Update()
{
	if (WinInput::Get()->GetKeyDown(VK_SPACE) && ableToJump)
	{
		ableToJump = false;
		weight = 1500;
	}
	weight += gravity;
	GetTransform()->localPosition.y += weight * Utility::UTime::Get()->DeltaTimeF();
	if (GetComponent<Physics2D::RectCollider>()->others.size() > 0)
	{
		if (GetTransform()->localPosition.y < -150)
			GetTransform()->localPosition.y = -150;
		ableToJump = true;
		if (weight < 0)
		{
			weight = 0;
		}
	}
	else
	{
		ableToJump = false;
	}
	lastpos = GetTransform()->localPosition;

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