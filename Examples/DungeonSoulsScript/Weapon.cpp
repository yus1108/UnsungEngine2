#include "pch.h"
#include "Weapon.h"

void Weapon::Update()
{
	auto position = Vector2(GetTransform()->GetTransform()->GetWorld().r[3]);
	auto mousePos = Math::GetMousePosToWorld();

	auto mouseDirection = (mousePos - position).Normalize();

	Console::Clear();
	Console::WriteLine(to_string(position.x) + ", " + to_string(position.y));
	Console::WriteLine(to_string(mousePos.x) + ", " + to_string(mousePos.y));
	Console::WriteLine(to_string(mouseDirection.x) + ", " + to_string(mouseDirection.y));
}