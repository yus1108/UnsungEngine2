#include "stdafx.h"
#include "Player.h"

void Player::Start()
{
	material = GetComponent<Material>();
}

void Player::FixedUpdate()
{
	weight += gravity;
	GetTransform()->localPosition.y += weight;
	if (GetTransform()->localPosition.y <= -150)
	{
		GetTransform()->localPosition.y = -150;
		weight = 0;
		ableToJump = true;
	}
}

void Player::Update()
{
	if (WinInput::Get()->GetKeyDown(VK_SPACE) && ableToJump)
	{
		ableToJump = false;
		weight = 80;
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
