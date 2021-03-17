#include "pch.h"
#include "Script.h"
#include <iostream>

Script* Script::isSelected = nullptr;

void Script::Update()
{
    float deltatime = UEngine::Utility::UTime::Get()->DeltaTimeF();
    //if (UEngine::WinInput::Get()->GetKey('W'))
    //{
    //    GetTransform()->localPosition.value.y += 2 * deltatime;
    //}
    //if (UEngine::WinInput::Get()->GetKey('A'))
    //{
    //    GetTransform()->localPosition.value.x -= 2 * deltatime;
    //}
    //if (UEngine::WinInput::Get()->GetKey('S'))
    //{
    //    GetTransform()->localPosition.value.y -= 2 * deltatime;
    //}
    //if (UEngine::WinInput::Get()->GetKey('D'))
    //{
    //    GetTransform()->localPosition.value.x += 2 * deltatime;
    //}

    
    auto mousePos = UEngine::Math::GetMousePosToWorld();
    if (UEngine::SingletonManager::Input->GetKey(VK_LBUTTON))
    {
        if (UEngine::Physics2D::IsColliding(mousePos, aabb))
        {
            isSelected = this;
        }
        else
        {
            if (isSelected != nullptr)
            {
                if (!UEngine::Physics2D::IsColliding(mousePos, isSelected->aabb))
                {
                    isSelected = nullptr;
                }
            }
        }
    }
    Console::Clear();
    Console::WriteLine(std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y));
    if (isSelected == this)
    {
        Console::WriteLine("colliding");
    }
}

void Script::LateUpdate()
{
    aabb = UEngine::Physics2D::MakeAABB(GetTransform()->GetWorld());
}

void Script::OnPreRender()
{
    if (isSelected == this)
    {
        auto matrix = DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(100, 100, 1), GetTransform()->GetRTP());
        GetGameObject()->GetScene()->debugRenderer->Add_Axis(matrix);
    }
   
}
