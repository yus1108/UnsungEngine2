#include "stdafx.h"
#include "Player2.h"

void Player2::Update()
{
	auto transform = GetTransform();

	if (transform->localPosition.value.y + transform->scale.value.x * 0.5f > 0)
		transform->localPosition.value.y = -transform->scale.value.x * 0.5f;
	if (transform->localPosition.value.x + transform->scale.value.x * 0.5f > 7.5f)
		transform->localPosition.value.x = 7.5f - transform->scale.value.x * 0.5f;
	if (transform->localPosition.value.x - transform->scale.value.x * 0.5f < -7.5f)
		transform->localPosition.value.x = transform->scale.value.x * 0.5f - 7.5f;
	if (transform->localPosition.value.y - transform->scale.value.x * 0.5f < -15.0f)
		transform->localPosition.value.y = transform->scale.value.x * 0.5f - 15.0f;

	if (WinInput::Get()->GetKey(VK_UP))
		transform->localPosition.value.y += Utility::UTime::Get()->DeltaTimeF() * speed;
	if (WinInput::Get()->GetKey(VK_DOWN))
		transform->localPosition.value.y -= Utility::UTime::Get()->DeltaTimeF() * speed;
	if (WinInput::Get()->GetKey(VK_RIGHT))
		transform->localPosition.value.x += Utility::UTime::Get()->DeltaTimeF() * speed;
	if (WinInput::Get()->GetKey(VK_LEFT))
		transform->localPosition.value.x -= Utility::UTime::Get()->DeltaTimeF() * speed;

	Player::Update();
}
