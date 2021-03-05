#include "stdafx.h"
#include "Puzzle.h"

void Puzzle::Update()
{
	if (WinInput::Get()->GetKey('1'))
	{
		pieces[0][0]->GetComponent<RenderComponent>()->SetEnable(false);
	}
}
