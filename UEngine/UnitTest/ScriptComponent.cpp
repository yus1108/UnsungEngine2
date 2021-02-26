#include "framework.h"
#include "ScriptComponent.h"

void ScriptComponent::Update()
{
	if (WinInput::Get()->GetKey(VK_RIGHT))
	{
		GetTransform()->localPosition.x += Utility::UTime::Get()->DeltaTimeF();
	}
	if (WinInput::Get()->GetKey(VK_LEFT))
	{
		GetTransform()->localPosition.x -= Utility::UTime::Get()->DeltaTimeF();
	}

	DebugVertex a, b;
	a = DebugVertex{ Vector3{0, 2, 0}, Color{ 0, 0, 1, 1} };
	b = DebugVertex{ Vector3{3, 2, 0}, Color{ 0, 0, 1, 1} };
	GameState::Get()->debugRenderer.Add_line(a, b);
	GameState::Get()->debugRenderer.Add_Axis(GetTransform()->GetWorld());
	std::cout << GetTransform()->GetWorld().r[3].m128_f32[0] << std::endl;
}
