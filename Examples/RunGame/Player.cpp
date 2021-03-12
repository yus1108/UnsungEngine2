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
		//GetTransform()->localPosition.y += weight;
		weight += gravity;
		lastpos = GetTransform()->localPosition;
	}
	if (hitCounter <= 0)
	{
		material->color = Color{ 1, 1, 1, 1 };
	}
	else
	{
		if (hitColor)
		{
			material->color = Color{ 1, 0, 0, 1 };
		}
		else
		{
			material->color = Color{ 1, 1, 1, 1 };
		}
		hitColor = !hitColor;
	}
}

void Player::Update()
{
	Vector2 mousepos = Math::GetMousePosToWorld();
	if (WinInput::Get()->GetKeyDown(VK_LBUTTON))
	{
		if (Math::Physics2D::IsColliding(mousepos, collider->GetCollider()))
		{
			hitCounter = hitMaxTimer;
		}
		
	}
	hitCounter -= Utility::UTime::Get()->DeltaTimeF();
	

	if (WinInput::Get()->GetKeyDown(VK_SPACE) && ableToJump && GetTransform()->localPosition.y < -155.0f)
	{
		ableToJump = false;
		weight = 20;
	}

	auto transform = GetTransform();
	if (WinInput::Get()->GetKey('1'))
	{
		GetTransform()->localPosition.x = -450;
		GetTransform()->localPosition.y = -100;
		weight = 0;
	}
	if (WinInput::Get()->GetKey(VK_DOWN))
	{
		auto value = Vector2(0, -1) * 200 * Utility::UTime::Get()->DeltaTimeF();
		transform->localPosition = transform->localPosition + value;
	}
	if (WinInput::Get()->GetKey(VK_UP))
	{
		auto value = Vector2(0, 1) * 200 * Utility::UTime::Get()->DeltaTimeF();
		transform->localPosition = transform->localPosition + value;
	}
	if (WinInput::Get()->GetKey(VK_RIGHT))
	{
		auto value = Vector2(1, 0) * 200 * Utility::UTime::Get()->DeltaTimeF();
		transform->localPosition = transform->localPosition + value;
	}
	if (WinInput::Get()->GetKey(VK_LEFT))
	{
		auto value = Vector2(-1, 0) * 200 * Utility::UTime::Get()->DeltaTimeF();
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
	GetGameObject()->GetScene()->partition2D.DebugRender(GetGameObject()->GetScene()->partition2D.head, GetComponent<Physics2D::RectCollider>(), Color{ 1, 0, 0, 1 }, Color{ 0, 0, 1, 1 });
	GetGameObject()->GetScene()->partition2D.DebugRender(GetGameObject()->GetScene()->partition2D.head, GetComponent<Physics2D::RectCollider>(), Color{ 1, 1, 0, 1 });
}