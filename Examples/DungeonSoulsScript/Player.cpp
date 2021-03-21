#include "pch.h"
#include "Player.h"

void Player::Start()
{
	imageObj = FindObjectWithName("playerImage");
	material = imageObj->GetComponent<Material>();
}

void Player::Update()
{
	auto transform = GetTransform();
	float deltaTime = Utility::UTime::Get()->DeltaTimeF();

	if (UEngine::Input::GetKey('S'))
	{
		auto value = Vector2(0, -1) * frameSize * speed * deltaTime;
		transform->localPosition.value = transform->localPosition.value + value;
	}
	if (UEngine::Input::GetKey('W'))
	{
		auto value = Vector2(0, 1) * frameSize * speed * deltaTime;
		transform->localPosition.value = transform->localPosition.value + value;
	}
	if (UEngine::Input::GetKey('D'))
	{
		auto value = Vector2(1, 0) * frameSize * speed * deltaTime;
		transform->localPosition.value = transform->localPosition.value + value;
	}
	if (UEngine::Input::GetKey('A'))
	{
		auto value = Vector2(-1, 0) * frameSize * speed * deltaTime;
		transform->localPosition.value = transform->localPosition.value + value;
	}
	if (UEngine::Input::GetKeyDown(VK_SPACE))
	{
		isJumping = !isJumping;
	}
	if (UEngine::Input::GetKeyDown(VK_SPACE)) 
	{
		isRolling = !isRolling;
	}
	if (Input::GetKeyDown(VK_LBUTTON))
	{
		isAttacking = !isAttacking;
	}

	Vector2 velocity = transform->localPosition.value - lastPos;
	if (isJumping)
	{
		animationState = PLAYER_ANIMATION_STATE_JUMP;
	}
	else if (isAttacking)
	{
		animationState = PLAYER_ANIMATION_STATE_ATTACK1;
		if (velocity.x < 0)
		{
			imageObj->GetTransform()->localRotation.value.y = -Utility::UMath::PI;
		}
		else if (velocity.x > 0)
		{
			imageObj->GetTransform()->localRotation.value.y = 0;
		}
	}
	else
	{
		if (velocity.x == 0 && velocity.y == 0)
		{
			animationState = PLAYER_ANIMATION_STATE_IDLE;
		}
		else
		{
			animationState = PLAYER_ANIMATION_STATE_MOVE;
			if (velocity.x < 0)
			{
				imageObj->GetTransform()->localRotation.value.y = -Utility::UMath::PI;
			}
			else if (velocity.x > 0)
			{
				imageObj->GetTransform()->localRotation.value.y = 0;
			}
		}
	}
	lastPos = transform->localPosition.value;

	if (timer >= player_animation_map[animationState].delay)
	{
		index++;
		if (index >= player_animation_map[animationState].numSprite)
			index = 0;
		material->uv = UV
		{
			(frameSize * index) / textureSize,
			static_cast<float>(animationState) / static_cast<float>(PLAYER_ANIMATION_STATE_COUNT),
			(frameSize * (index + 1)) / textureSize,
			static_cast<float>(animationState + 1) / static_cast<float>(PLAYER_ANIMATION_STATE_COUNT)
		};
		timer = 0;
		if (index == 10)
		{
			index = 0;
		}
	}
	timer += deltaTime;
}