#include "stdafx.h"
#include "Player1.h"

using namespace UEngine;
void Player1::Update()
{
	auto transform = GetTransform();

	if (transform->localPosition.y - transform->scale.x * 0.5f < 0)
		transform->localPosition.y = transform->scale.x * 0.5f;
	if (transform->localPosition.x + transform->scale.x * 0.5f > 7.5f)
		transform->localPosition.x = 7.5f - transform->scale.x * 0.5f;
	if (transform->localPosition.x - transform->scale.x * 0.5f < -7.5f)
		transform->localPosition.x = transform->scale.x * 0.5f - 7.5f;
	if (transform->localPosition.y + transform->scale.x * 0.5f > 15.0f)
		transform->localPosition.y = 15.0f - transform->scale.x * 0.5f;

	if (WinInput::Get()->GetKey('W'))
		transform->localPosition.y += Utility::UTime::Get()->DeltaTimeF() * speed;
	if (WinInput::Get()->GetKey('S'))									  
		transform->localPosition.y -= Utility::UTime::Get()->DeltaTimeF() * speed;
	if (WinInput::Get()->GetKey('D'))									  
		transform->localPosition.x += Utility::UTime::Get()->DeltaTimeF() * speed;
	if (WinInput::Get()->GetKey('A'))									  
		transform->localPosition.x -= Utility::UTime::Get()->DeltaTimeF() * speed;

	

	Player::Update();
}