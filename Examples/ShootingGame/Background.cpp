#include "stdafx.h"
#include "Background.h"

void Background::Start()
{
	material = GetComponent<Material>();
}

void Background::Update()
{
	material->uv = UV
	{
		0,
		timer,
		1,
		timer + mapSize
	};

	timer -= Utility::UTime::Get()->DeltaTimeF() * speed;
}
