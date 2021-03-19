#include "UEngine.h"
#include "EditorScript.h"

namespace UEngine
{
    EditorScript* EditorScript::isSelected = nullptr;
    bool EditorScript::isKeyDown = false;

    void EditorScript::Update()
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
    }

    void EditorScript::LateUpdate()
    {
        aabb = UEngine::Physics2D::MakeAABB(GetTransform()->GetWorld());
    }

    void EditorScript::OnPreRender()
    {
        if (isSelected == this)
        {
            auto gameObject = GetGameObject();
            auto transform = GetTransform();

            auto matrix = DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(100, 100, 1), transform->GetRTP());
            gameObject->GetScene()->debugRenderer->Add_Axis(matrix);
            gameObject->GetScene()->debugRenderer->Add_Rectangle(aabb, UEngine::Color{ 0.5f, 0.5f, 0.5f, 1 });

            ImGui::PushID(this);
            ImGui::Begin("Inspector");

            ImGui::Text("GameObject");

            size_t nameSize = gameObject->name.size() + 1;
            
            char* nameBuffer = new char[nameSize + 20];
            strcpy_s(nameBuffer, nameSize, gameObject->name.c_str());
            ImGui::InputText("Name", nameBuffer, nameSize + 20);
            gameObject->name = std::string(nameBuffer);
            delete[] nameBuffer;

            bool active = gameObject->GetActive();
            ImGui::Checkbox("Active", &active);  
            gameObject->SetActive(active);

            ImGui::SameLine();

            bool isStatic = gameObject->IsStatic;
            ImGui::Checkbox("Static", &isStatic);
            gameObject->IsStatic = isStatic;

            ImGui::Separator();

            for (auto component : gameObject->GetComponents())
            {
                if (component->typeName == ".PAVEditorScript@UEngine@@") continue;
                auto typeName = component->typeName.substr(4, component->typeName.find_first_of('@') - 4);
                ImGui::PushID(component);
                if (ImGui::CollapsingHeader(typeName.c_str()))
                {
                    component->OnEditRender();
                }
                ImGui::PopID();
            }

            if (!isAddingComponent)
            {
                isAddingComponent = ImGui::Button("Add Component");
            }
            if (isAddingComponent)
            {
                ImGui::Separator();
                static ImGuiTextFilter filter;
                const char* renderingItems[] = 
                { 
                    "RenderComponent",
                    "Material", 
                    "Camera",
                    "Physics2D"
                    "CircleCollider",
                    "RectCollider"
                };
                // TODO: Dynamically find all user defined scripts
                static int item_current_idx = 0; // Here we store our selection data as an index.

                if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
                {
                    for (int n = 0; n < IM_ARRAYSIZE(renderingItems); n++)
                    {
                        if (filter.PassFilter(renderingItems[n]))
                        {
                            const bool is_selected = (item_current_idx == n);
                            if (ImGui::Selectable(renderingItems[n], is_selected))
                                item_current_idx = n;

                            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        
                    }
                    ImGui::EndListBox();
                }
                filter.Draw("");
                ImGui::Button("Add Component");
            }

            ImGui::End();
            ImGui::PopID();
        }

    }

    void EditorScript::OnDestroy()
    {
        if (isSelected == this)
        {
            isSelected = nullptr;
            isMoving = false;
            isKeyDown = false;
        }
    }

}