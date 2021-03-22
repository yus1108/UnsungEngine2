#pragma once

class Animation
{
private:
	POINT index = { 0, 0 };
	bool stop = false;
	float timer = 0;
public:
	bool repeat = false;
	float delay = 1;
	POINT startIndex;
	POINT endIndex;
	Vector2 frameSize;
	Vector2 textureSize;
	Vector2 offset;

	Animation() = default;
	Animation(bool repeat, float delay, POINT startIndex, POINT endIndex, Vector2 frameSize, Vector2 textureSize, Vector2 offset)
		: repeat(repeat), delay(delay), startIndex(startIndex), endIndex(endIndex), frameSize(frameSize), textureSize(textureSize), offset(offset)
	{
		index = startIndex;
	}
	virtual ~Animation() = default;

	bool operator==(Animation& rhs);
	bool operator!=(Animation& rhs);

	UV Update();
	virtual void Stop() { stop = true; }
	virtual void Play() { stop = false; }
	virtual void Change(Animation& animation);
	virtual bool IsAt(POINT spriteNum) { return index.x == spriteNum.x && index.y == spriteNum.y; }
	virtual bool IsPassed(POINT spriteNum) { return index.x >= spriteNum.x && index.y >= spriteNum.y; }
	virtual bool IsFinished() { return index.x >= endIndex.x - 1 && index.y >= endIndex.y - 1; }
};

