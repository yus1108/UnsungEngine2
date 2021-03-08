#include "stdafx.h"
#include "Tile.h"

void Tile::Update()
{
	GetTransform()->localPosition.x -= 500 * Utility::UTime::Get()->DeltaTimeF();
	if (GetTransform()->localPosition.x < -650)
	{
		GetTransform()->localPosition.x += 1300;
	}
}
