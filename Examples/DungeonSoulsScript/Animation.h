#pragma once

class Animation
{
private:
	POINT index;

public:
	Vector2 frameSize;
	Vector2 textureSize;
	Vector2 numSprite;
	float timer = 0;
	float delay = 0;

	UV Update();
};

