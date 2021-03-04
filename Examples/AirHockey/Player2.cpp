#include "stdafx.h"
#include "Player2.h"

void Player2::Update()
{
	auto transform = GetTransform();

	if (transform->localPosition.y + transform->scale.x * 0.5f > 0)
		transform->localPosition.y = -transform->scale.x * 0.5f;
	if (transform->localPosition.x + transform->scale.x * 0.5f > 7.5f)
		transform->localPosition.x = 7.5f - transform->scale.x * 0.5f;
	if (transform->localPosition.x - transform->scale.x * 0.5f < -7.5f)
		transform->localPosition.x = transform->scale.x * 0.5f - 7.5f;
	if (transform->localPosition.y - transform->scale.x * 0.5f < -15.0f)
		transform->localPosition.y = transform->scale.x * 0.5f - 15.0f;

	if (WinInput::Get()->GetKey(VK_UP))
		transform->localPosition.y += Utility::UTime::Get()->DeltaTimeF() * speed;
	if (WinInput::Get()->GetKey(VK_DOWN))
		transform->localPosition.y -= Utility::UTime::Get()->DeltaTimeF() * speed;
	if (WinInput::Get()->GetKey(VK_RIGHT))
		transform->localPosition.x += Utility::UTime::Get()->DeltaTimeF() * speed;
	if (WinInput::Get()->GetKey(VK_LEFT))
		transform->localPosition.x -= Utility::UTime::Get()->DeltaTimeF() * speed;

	Player::Update();
}
