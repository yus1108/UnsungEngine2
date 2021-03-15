#include "stdafx.h"
#include "Player1.h"

using namespace UEngine;
void Player1::Update()
{
	auto transform = GetTransform();

	if (transform->localPosition.value.y - transform->scale.value.x * 0.5f < 0)
		transform->localPosition.value.y = transform->scale.value.x * 0.5f;
	if (transform->localPosition.value.x + transform->scale.value.x * 0.5f > 7.5f)
		transform->localPosition.value.x = 7.5f - transform->scale.value.x * 0.5f;
	if (transform->localPosition.value.x - transform->scale.value.x * 0.5f < -7.5f)
		transform->localPosition.value.x = transform->scale.value.x * 0.5f - 7.5f;
	if (transform->localPosition.value.y + transform->scale.value.x * 0.5f > 15.0f)
		transform->localPosition.value.y = 15.0f - transform->scale.value.x * 0.5f;

	if (WinInput::Get()->GetKey('W'))
		transform->localPosition.value.y += Utility::UTime::Get()->DeltaTimeF() * speed;
	if (WinInput::Get()->GetKey('S'))									  
		transform->localPosition.value.y -= Utility::UTime::Get()->DeltaTimeF() * speed;
	if (WinInput::Get()->GetKey('D'))									  
		transform->localPosition.value.x += Utility::UTime::Get()->DeltaTimeF() * speed;
	if (WinInput::Get()->GetKey('A'))									  
		transform->localPosition.value.x -= Utility::UTime::Get()->DeltaTimeF() * speed;

	Player::Update();
}