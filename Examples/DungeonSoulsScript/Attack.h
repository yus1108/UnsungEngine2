#pragma once

class Attack : public Animation
{
	long yIndex;
public:

	Attack() = default;
	Attack(Attack& attack) = default;
	Attack(Animation& animation) : Animation(animation) {}
	Attack(long yIndex)
		: Animation(false, 0.1f, { 0, yIndex }, { 10, yIndex }, { 32.0f, 32.0f }, { 416.0f, 416.0f }, { 5.0f, 0 })
		, yIndex(yIndex)
	{}

	float ReceiveInput(Animation& animation, Animation NextCombo, float dashValue);
};

