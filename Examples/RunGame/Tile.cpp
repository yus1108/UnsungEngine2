#include "stdafx.h"
#include "Tile.h"

void Tile::FixedUpdate()
{
	if (GetTransform()->localPosition.x < -650)
	{
		GetTransform()->localPosition.x += 1300;
		if (Math::RndInt())
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
				GetComponent<Physics2D::RectCollider>()->IsTrigger = true;
			}
		}
	}
	GetTransform()->localPosition.y = -250;
}

void Tile::Update()
{
	lastpos = GetTransform()->localPosition;
}
