#include "pch.h"
#include "Weapon.h"
#include <math.h>

void Weapon::Update()
{
	auto position = FindObjectWithName("player")->GetTransform()->localPosition.value;
	auto mousePos = Math::GetMousePosToWorld();

	auto mouseDirection = (mousePos - position).Normalize();

	GetTransform()->localPosition = mouseDirection * 10.0f;
	GetTransform()->localPosition.value.y -= 5.0f;

	//if (Input::GetKeyDown(VK_LBUTTON))
	//{
	//	isAttack = !isAttack;
	//}

}