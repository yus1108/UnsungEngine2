#include "stdafx.h"
#include "Tile.h"

void Tile::Start()
{
	GetTransform()->localPosition.x = fixedPosition.x;
}
void Tile::FixedUpdate()
{
	fixedPosition.x -= 3;
	if (GetTransform()->localPosition.x < -650)
	{
		fixedPosition.x += 1300;
		if (Math::RndInt(0, 3) % 4 == 0)
		{
			if (GetComponent<RenderComponent>()->GetEnable())
			{
				GetComponent<RenderComponent>()->SetEnable(false);
				RemoveComponent<Physics2D::RectCollider>();
			}
		}
		else
		{
			if (!GetComponent<RenderComponent>()->GetEnable())
			{
				GetComponent<RenderComponent>()->SetEnable(true);
				AddComponent<Physics2D::RectCollider>()->SetCollider(100, 100);
			}
		}
	}
}

void Tile::Update()
{
	GetTransform()->localPosition.x = fixedPosition.x;
	GetTransform()->localPosition.y = -250;
	//auto transform = GetTransform();
	//if (WinInput::Get()->GetKey('S'))
	//{
	//	auto value = Vector2(0, -1) * 400 * Utility::UTime::Get()->DeltaTimeF();
	//	transform->localPosition = transform->localPosition + value;
	//}
	//if (WinInput::Get()->GetKey('W'))
	//{
	//	auto value = Vector2(0, 1) * 400 * Utility::UTime::Get()->DeltaTimeF();
	//	transform->localPosition = transform->localPosition + value;
	//}
	//if (WinInput::Get()->GetKey('D'))
	//{
	//	auto value = Vector2(1, 0) * 400 * Utility::UTime::Get()->DeltaTimeF();
	//	transform->localPosition = transform->localPosition + value;
	//}
	//if (WinInput::Get()->GetKey('A'))
	//{
	//	auto value = Vector2(-1, 0) * 400 * Utility::UTime::Get()->DeltaTimeF();
	//	transform->localPosition = transform->localPosition + value;
	//}
}
