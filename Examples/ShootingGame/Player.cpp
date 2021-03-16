#include "stdafx.h"
#include "Player.h"

void Player::Start()
{
	material = GetComponent<Material>();
	collider = GetComponent<Physics2D::RectCollider>();
	currMapIndex = startIndex;
}

void Player::FixedUpdate()
{
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
	
	auto transform = GetTransform();

	if (WinInput::Get()->GetKey(VK_DOWN))
	{
		auto value = Vector2(0, -1) * 200 * Utility::UTime::Get()->DeltaTimeF();
		transform->localPosition.value = transform->localPosition.value + value;
	}
	if (WinInput::Get()->GetKey(VK_UP))
	{
		auto value = Vector2(0, 1) * 200 * Utility::UTime::Get()->DeltaTimeF();
		transform->localPosition.value = transform->localPosition.value + value;
	}
	if (WinInput::Get()->GetKey(VK_RIGHT))
	{
		auto value = Vector2(1, 0) * 200 * Utility::UTime::Get()->DeltaTimeF();
		transform->localPosition.value = transform->localPosition.value + value;
		startIndex.x = startIndex.x == 4 ? 4 : startIndex.x + 1;
		endIndex.x = endIndex.x == 4 ? 4 : endIndex.x + 1;
	}
	if (WinInput::Get()->GetKey(VK_LEFT))
	{
		auto value = Vector2(-1, 0) * 200 * Utility::UTime::Get()->DeltaTimeF();
		transform->localPosition.value = transform->localPosition.value + value;
		startIndex.x = startIndex.x == 0 ? 0 : startIndex.x - 1;
		endIndex.x = endIndex.x == 0 ? 0 : endIndex.x - 1;
	}

	if (WinInput::Get()->GetKey(VK_RIGHT))
	{
		reverse = false;
	}
	else if (WinInput::Get()->GetKey(VK_LEFT))
	{
		reverse = true;
	}
	else
	{
		if (currMapIndex.x < 2)
			reverse = false;
		else if (currMapIndex.x > 2)
			reverse = true;

		startIndex.x = 2;
		endIndex.x = 2;
	}

	
	if (reverse)
	{
		if (frame == static_cast<float>(maxSpriteFrame))
			currMapIndex.x = currMapIndex.x <= startIndex.x ? endIndex.x : currMapIndex.x - 1;
	}
	else
	{
		if (frame == static_cast<float>(maxSpriteFrame))
			currMapIndex.x = currMapIndex.x >= endIndex.x ? startIndex.x : currMapIndex.x + 1;
	}
	
	if (frame == static_cast<float>(maxSpriteFrame) / (endIndex.y - startIndex.y))
		currMapIndex.y = currMapIndex.y >= endIndex.y ? startIndex.y : currMapIndex.y + 1;

	material->uv = UV
	{
		static_cast<float>(currMapIndex.x) / 5.0f,
		static_cast<float>(currMapIndex.y) / 2.0f,
		static_cast<float>(currMapIndex.x + 1) / 5.0f,
		static_cast<float>(currMapIndex.y + 1) / 2.0f,
	};

	if (frame == maxSpriteFrame)
		frame = 0;
	frame++;

	if (movingCounter > 0.5f)
	{
		movingCounter = 0;
	}
	movingCounter += Utility::UTime::Get()->DeltaTimeF();
}

void Player::OnPreRender()
{
	GetGameObject()->GetScene()->partition2D->DebugRender(GetGameObject()->GetScene()->partition2D->head, GetComponent<Physics2D::RectCollider>(), Color{ 1, 0, 0, 1 }, Color{ 0, 0, 1, 1 });
	GetGameObject()->GetScene()->partition2D->DebugRender(GetGameObject()->GetScene()->partition2D->head, GetComponent<Physics2D::RectCollider>(), Color{ 1, 1, 0, 1 });
}