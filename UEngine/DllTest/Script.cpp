#include "pch.h"
#include "Script.h"
#include <iostream>

void Script::Update()
{
    float deltatime = UEngine::Utility::UTime::Get()->DeltaTimeF();
    std::cout << UEngine::Utility::UTime::Get()->DeltaTimeF() << std::endl;
    if (UEngine::WinInput::Get()->GetKey('W'))
    {
        GetTransform()->localPosition.value.y += 2 * deltatime;
    }
    if (UEngine::WinInput::Get()->GetKey('A'))
    {
        GetTransform()->localPosition.value.x -= 2 * deltatime;
    }
    if (UEngine::WinInput::Get()->GetKey('S'))
    {
        GetTransform()->localPosition.value.y -= 2 * deltatime;
    }
    if (UEngine::WinInput::Get()->GetKey('D'))
    {
        GetTransform()->localPosition.value.x += 2 * deltatime;
    }
}
