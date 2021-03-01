#include "framework.h"
#include "ScriptComponent.h"

void ScriptComponent::Update()
{
	using namespace std;
	using namespace DirectX;

	if (!isSelected)
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

	//if (WinInput::Get()->GetKey(VK_LBUTTON))
	//{
	//	if (Math::Physics2D::IsColliding(mousePos, circle))
	//		isSelected = true;
	//}

	auto mousePos = Math::GetMousePosToWorld();
	auto worldMatrix = GetTransform()->GetWorld();

	auto triangle = Math::Physics2D::MakeTriangle(worldMatrix);
	auto aabb = Math::Physics2D::MakeAABB(worldMatrix);
	auto circle = Math::Physics2D::MakeCircle(Vector2(), 0.5f);

	bool isColliding = Math::Physics2D::IsColliding(aabb, circle);
	UEngine::WinConsole::ResetCursorPos();
	std::cout << "\t\t\t\t" << std::endl;
	std::cout << "\t\t\t\t" << std::endl;
	std::cout << "\t\t\t\t" << std::endl;
	UEngine::WinConsole::ResetCursorPos();
	std::cout << mousePos.x << ", " << mousePos.y << std::endl;
	//std::cout << coords[2].x << ", " << coords[2].y << std::endl;
	std::cout << isColliding << std::endl;
}

void ScriptComponent::OnPreRender()
{
	auto worldMatrix = GetTransform()->GetWorld();
	
	auto triangle = Math::Physics2D::MakeTriangle(worldMatrix);
	auto triangleAABB = Math::Physics2D::MakeAABB(Math::Physics2D::MakePointVector(&triangle[0], 3));
	auto aabb = Math::Physics2D::MakeAABB(MatrixIdentity());
	auto circle = Math::Physics2D::MakeCircle(Vector2(), 0.5f);

	auto color = Color{ 1, 0, 0, 1 };
	GameState::Get()->debugRenderer.Add_Axis(GetTransform()->GetWorld());
	//GameState::Get()->debugRenderer.Add_Triangle(triangle, color);
	GameState::Get()->debugRenderer.Add_Rectangle(aabb, color);
	GameState::Get()->debugRenderer.Add_Rectangle(triangleAABB, color);
	GameState::Get()->debugRenderer.Add_Circle(circle, color);
}
