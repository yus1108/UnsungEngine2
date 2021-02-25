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
}
