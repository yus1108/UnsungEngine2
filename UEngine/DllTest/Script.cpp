#include "pch.h"
#include "Script.h"
#include <iostream>

void Script::Update()
{
    float deltatime = UEngine::Utility::UTime::Get()->DeltaTimeF();
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

    auto mousePos = UEngine::Math::GetMousePosToWorld();
    if (UEngine::SingletonManager::Input->GetKey(VK_LBUTTON))
    {
        if (UEngine::Physics2D::IsColliding(mousePos, aabb))
            isSelected = true;
        else
            isSelected = false;
    }
    
    Console::Clear();
    Console::WriteLine(std::to_string(isSelected.value));
}

void Script::LateUpdate()
{
    aabb = UEngine::Physics2D::MakeAABB(GetTransform()->GetWorld());
}

void Script::OnPreRender()
{
    GetGameObject()->GetScene()->debugRenderer->Add_Axis(GetTransform()->GetWorld());
}
