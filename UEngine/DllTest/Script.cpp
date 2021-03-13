#include "pch.h"
#include "Script.h"
#include <iostream>

void Script::Update()
{
    std::cout << UEngine::Utility::UTime::Get()->DeltaTimeF() << std::endl;
    if (UEngine::WinInput::Get()->GetKeyDown('W'))
    {
        GetTransform()->localPosition.y += 1;
    }
}
