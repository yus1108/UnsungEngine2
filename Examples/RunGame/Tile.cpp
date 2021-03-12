#include "stdafx.h"
#include "Tile.h"

void Tile::FixedUpdate()
{
	//fixedPosition.x -= 7;
	if (GetTransform()->localPosition.x < -650)
	{
		fixedPosition.x += 1300;
		/*if (Math::RndInt())
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
		}*/
	}
}

void Tile::Update()
{
	//GetTransform()->localPosition.x = fixedPosition.x;
	//GetTransform()->localPosition.y = -250;
}
