#include "pch.h"
#include "Flight.h"

void Flight::Start()
{
	material = GetComponent<Material>();
	currMapIndex = startIndex;
}

void Flight::Update()
{
	auto transform = GetTransform();

	if (UEngine::Input::GetKey(VK_DOWN))
	{
		auto value = Vector2(0, -1) * 50 * Utility::UTime::Get()->DeltaTimeF();
		transform->localPosition.value = transform->localPosition.value + value;
	}
	if (UEngine::Input::GetKey(VK_UP))
	{
		auto value = Vector2(0, 1) * 50 * Utility::UTime::Get()->DeltaTimeF();
		transform->localPosition.value = transform->localPosition.value + value;
	}
	if (UEngine::Input::GetKey(VK_RIGHT))
	{
		auto value = Vector2(1, 0) * 50 * Utility::UTime::Get()->DeltaTimeF();
		transform->localPosition.value = transform->localPosition.value + value;
		if (frame == maxSpriteFrame)
			currMapIndex.x = currMapIndex.x == endIndex.x - 1 ? endIndex.x - 1 : currMapIndex.x + 1;
	}
	if (UEngine::Input::GetKey(VK_LEFT))
	{
		auto value = Vector2(-1, 0) * 50 * Utility::UTime::Get()->DeltaTimeF();
		transform->localPosition.value = transform->localPosition.value + value;
		if (frame == maxSpriteFrame)
			currMapIndex.x = currMapIndex.x == 0 ? 0 : currMapIndex.x - 1;
	}

	if (UEngine::Input::GetKey(VK_RIGHT))
	{
		reverse = false;
	}
	if (UEngine::Input::GetKey(VK_LEFT))
	{
		reverse = true;
	}
	else
	{
		if (currMapIndex.x < 3)
		{
			reverse = false;
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
		}
		else if (currMapIndex.x > 3)
		{
			reverse = true;
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
		}
	}

	
	if (frame == static_cast<float>(maxSpriteFrame) / (endIndex.y - startIndex.y))
		currMapIndex.y = currMapIndex.y >= endIndex.y ? startIndex.y : currMapIndex.y + 1;

	material->uv = UV
	{
		static_cast<float>(currMapIndex.x) / 7.0f,
		static_cast<float>(currMapIndex.y),
		static_cast<float>(currMapIndex.x + 1) / 7.0f,
		static_cast<float>(currMapIndex.y + 1),
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