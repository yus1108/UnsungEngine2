#include "pch.h"
#include "Script.h"
#include <iostream>

Script* Script::isSelected = nullptr;
bool Script::isKeyDown = false;

void Script::Update()
{
    float deltatime = UEngine::Utility::UTime::Get()->DeltaTimeF();
        
#pragma region Selection_And_Moving
    

    UEngine::Vector2 mousePos;
    if (UEngine::Input::IsMouseInWindow(mousePos) && UEngine::Input::GetKey(VK_LBUTTON))
    {
        mousePos = UEngine::Math::GetMousePosToWorld();
        if (!isKeyDown)
        {
            if (UEngine::Physics2D::IsColliding(mousePos, aabb))
            {
                isSelected = this;
                isKeyDown = true;
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
       
        if (isSelected == this)
        {
            if (UEngine::Input::GetKey(VK_UP))
            {
                GetTransform()->localPosition.value.y += deltatime;
            }
            if (UEngine::Input::GetKey(VK_LEFT))
            {
                GetTransform()->localPosition.value.x -= deltatime;
            }
            if (UEngine::Input::GetKey(VK_DOWN))
            {
                GetTransform()->localPosition.value.y -= deltatime;
            }
            if (UEngine::Input::GetKey(VK_RIGHT))
            {
                GetTransform()->localPosition.value.x += deltatime;
            }
            if (isMoving)
            {
                GetTransform()->localPosition = mousePos + posDiff;
            }
            else
            {
                isMoving = true;
                posDiff = GetTransform()->localPosition.value - mousePos;
            }
           
        }
    }
    else
    {
        isMoving = false;
        isKeyDown = false;
    }

#pragma endregion


    if (isSelected == this)
    {
        Console::Clear();
        Console::WriteLine(std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y));
        Console::WriteLine("colliding");
        Console::WriteLine(isMoving ? "true" : "false");
    }
    else if (isSelected == nullptr)
    {
        Console::Clear();
        Console::WriteLine(std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y));
        Console::WriteLine(UEngine::Input::IsMouseInWindow(mousePos) ? "true" : "false");
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
        GetGameObject()->GetScene()->debugRenderer->Add_Rectangle(aabb, UEngine::Color{ 0.5f, 0.5f, 0.5f, 1 });
    }
   
}
