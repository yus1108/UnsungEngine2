#include "stdafx.h"
#include "Background.h"

void Background::Start()
{
	material = GetComponent<Material>();
}

void Background::Update()
{
	//if (frame == static_cast<float>(maxSpriteFrame) / maxMapIndex.x)
	//	currMapIndex.x = (currMapIndex.x + 1) % maxMapIndex.x;

	//if (frame == static_cast<float>(maxSpriteFrame) / maxMapIndex.y)
	//	currMapIndex.y = (currMapIndex.y + 1) % maxMapIndex.y;

	material->uv = UV
	{
		timer,
		0,
		timer + mapSize,
		1
	};

	timer += Utility::UTime::Get()->DeltaTimeF() * speed;
}
