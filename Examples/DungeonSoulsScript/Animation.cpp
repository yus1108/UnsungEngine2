#include "pch.h"
#include "Animation.h"

void Animation::Update()
{
	UV currUV;
	if (timer >= delay)
	{
		index.x++;
		if (index.x >= numSprite.x)
			index.x = 0;
		currUV = UV
		{
			(frameSize.x * index.x) / textureSize.x,
			(frameSize.y * index.y) / textureSize.y,
			(frameSize.x * (index.x + 1)) / textureSize.x,
			(frameSize.y * (index.y + 1)) / textureSize.y,
		};
		timer = 0;
		if (index == 10)
		{
			index = 0;
		}
	}
	timer += deltaTime;
}
