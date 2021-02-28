#include "framework.h"
#include "ScriptComponent.h"

void ScriptComponent::Update()
{
	using namespace std;
	using namespace DirectX;

	if (isSelected)
	{
		if (WinInput::Get()->GetKey(VK_RIGHT))
			GetTransform()->localPosition.x += Utility::UTime::Get()->DeltaTimeF();
		if (WinInput::Get()->GetKey(VK_LEFT))
			GetTransform()->localPosition.x -= Utility::UTime::Get()->DeltaTimeF();
		if (WinInput::Get()->GetKey(VK_UP))
			GetTransform()->localPosition.y += Utility::UTime::Get()->DeltaTimeF();
		if (WinInput::Get()->GetKey(VK_DOWN))
			GetTransform()->localPosition.y -= Utility::UTime::Get()->DeltaTimeF();
		if (WinInput::Get()->GetKey(VK_ESCAPE))
			isSelected = false;
	}
	if (WinInput::Get()->GetKey('Q'))
		GetTransform()->localRotation.y += Utility::UTime::Get()->DeltaTimeF();
	if (WinInput::Get()->GetKey('E'))
		GetTransform()->localRotation.y -= Utility::UTime::Get()->DeltaTimeF();

	auto mousePos = Math::GetMousePosToWorld();
	auto matrix = GetTransform()->GetWorld();

	Math::Physics2D::TriangleCoords coords;
	auto vertices = DXRenderer::DXResourceManager::Get()->GetVertices("triangle");
	for (UINT i = 0; i < 3; i++)
		coords[i] = Vector2(vertices[i].pos) * matrix;
	
	if (WinInput::Get()->GetKey(VK_LBUTTON))
	{
		if (Math::Physics2D::IsColliding(mousePos, coords))
			isSelected = true;
	}
	bool isColliding = Math::Physics2D::IsColliding(mousePos, coords);
	UEngine::WinConsole::ResetCursorPos();
	std::cout << "\t\t\t\t" << std::endl;
	std::cout << "\t\t\t\t" << std::endl;
	std::cout << "\t\t\t\t" << std::endl;
	UEngine::WinConsole::ResetCursorPos();
	std::cout << mousePos.x << ", " << mousePos.y << std::endl;
	std::cout << isColliding << std::endl;
}

void ScriptComponent::OnPreRender()
{
	GameState::Get()->debugRenderer.Add_Axis(GetTransform()->GetWorld());
}
