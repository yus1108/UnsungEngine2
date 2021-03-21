#include "pch.h"
#include "Animation.h"

bool Animation::operator==(Animation& rhs)
{
	if (
		this->repeat == rhs.repeat &&
		this->startIndex.x == rhs.startIndex.x &&
		this->startIndex.y == rhs.startIndex.y &&
		this->endIndex.x == rhs.endIndex.x &&
		this->endIndex.y == rhs.endIndex.y &&
		this->frameSize == rhs.frameSize &&
		this->textureSize == rhs.textureSize
		)
	{
		return true;
	}
	return false;
}

bool Animation::operator!=(Animation& rhs)
{
	return !((*this) == rhs);
}

UV Animation::Update()
{
	UV currUV = UV
	{
		(frameSize.x * index.x) / textureSize.x,
		(frameSize.y * index.y) / textureSize.y,
		(frameSize.x * (index.x + 1)) / textureSize.x,
		(frameSize.y * (index.y + 1)) / textureSize.y,
	};

	if (!stop)
	{
		if (timer >= delay)
		{
			index.x++;
			index.y++;
			if (repeat)
			{
				if (index.x >= endIndex.x - startIndex.x)
					index.x = startIndex.x;
				if (index.y >= endIndex.y - startIndex.y)
					index.y = startIndex.y;
			}
			else
			{
				if (index.x >= endIndex.x - startIndex.x)
					index.x = endIndex.x == startIndex.x ? startIndex.x : endIndex.x - 1;
				if (index.y >= endIndex.y - startIndex.y)
					index.y = endIndex.y == startIndex.y ? startIndex.y : endIndex.y - 1;
			}

			timer = 0;
		}
		timer += Utility::UTime::Get()->DeltaTimeF();
	}

	

	return currUV;
}

void Animation::Change(Animation& animation)
{
	if ((*this) != animation) this->index = { 0, 0 };

	this->repeat = animation.repeat;
	this->delay = animation.delay;
	this->startIndex = animation.startIndex;
	this->endIndex = animation.endIndex;
	this->frameSize = animation.frameSize;
	this->textureSize = animation.textureSize;
}
