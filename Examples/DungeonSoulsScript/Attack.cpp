#include "pch.h"
#include "Attack.h"

float Attack::ReceiveInput(Animation& animation, Animation NextCombo, int yIndex)
{
	// TODO: IMPLEMENT JUMP COMBO ATTACK
	if (animation.IsPassed({ 5, yIndex }))
	{
		animation.Change(NextCombo);
		return 1.0f;
	}

	return 0.0f;
}
