#include "pch.h"
#include "Attack.h"

float Attack::ReceiveInput(Animation& animation, Animation NextCombo, float dashValue)
{
	// TODO: IMPLEMENT JUMP COMBO ATTACK
	if (animation.IsPassed({ 5, yIndex }))
	{
		animation.Change(NextCombo);
		return dashValue;
	}

	return 0.0f;
}
