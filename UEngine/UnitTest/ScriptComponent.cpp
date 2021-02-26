#include "framework.h"
#include "ScriptComponent.h"

void ScriptComponent::Update()
{
	if (WinInput::Get()->GetKey(VK_RIGHT))
	{
		GetTransform()->localPosition.x += Utility::UTime::Get()->DeltaTime();
	}
	if (WinInput::Get()->GetKey(VK_LEFT))
	{
		GetTransform()->localPosition.x -= Utility::UTime::Get()->DeltaTime();
	}

	DebugVertex a, b;
	a = DebugVertex{ Vector3{0, 2, 0}, Color{ 0, 0, 1, 1} };
	b = DebugVertex{ Vector3{3, 2, 0}, Color{ 0, 0, 1, 1} };
	GameState::Get()->debugRenderer.Add_line(a, b);
}
