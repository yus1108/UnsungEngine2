#include "framework.h"
#include "ScriptComponent.h"

SpatialPartitioning ScriptComponent::sp;

void ScriptComponent::Start()
{
	auto x = (float)rand() / (rand() % INT_MAX);
	auto y = (float)rand() / (rand() % INT_MAX);
	float speed = UEngine::Math::RndFloat(1.0f, 5.0f);
	float scale = UEngine::Math::RndFloat(1.0f, 3.0f);
	GetTransform()->scale = Vector3(scale, scale, 1);
	dir.x = x;
	dir.y = y;
	dir = dir.Normalize();
	dir = dir * speed;
}

void ScriptComponent::Update()
{
	using namespace std;

	/*if (WinInput::Get()->GetKey(VK_RIGHT))
		GetTransform()->localPosition.x += Utility::UTime::Get()->DeltaTimeF();
	if (WinInput::Get()->GetKey(VK_LEFT))
		GetTransform()->localPosition.x -= Utility::UTime::Get()->DeltaTimeF();
	if (WinInput::Get()->GetKey(VK_UP))
		GetTransform()->localPosition.y += Utility::UTime::Get()->DeltaTimeF();
	if (WinInput::Get()->GetKey(VK_DOWN))
		GetTransform()->localPosition.y -= Utility::UTime::Get()->DeltaTimeF();
	if (WinInput::Get()->GetKey(VK_ESCAPE))
	if (WinInput::Get()->GetKey('Q'))
		GetTransform()->localRotation.y += Utility::UTime::Get()->DeltaTimeF();
	if (WinInput::Get()->GetKey('E'))
		GetTransform()->localRotation.y -= Utility::UTime::Get()->DeltaTimeF();*/

	auto transform = GetTransform();
	//auto value = dir * Utility::UTime::Get()->DeltaTimeF();
	//transform->localPosition = transform->localPosition + value;

	
	if (transform->localPosition.x + transform->scale.x * 0.5f > 20.0f) dir.x = -abs(dir.x);
	if (transform->localPosition.x - transform->scale.x * 0.5f < -20.0f) dir.x = abs(dir.x);
	if (transform->localPosition.y + transform->scale.x * 0.5f > 20.0f) dir.y = -abs(dir.y);
	if (transform->localPosition.y - transform->scale.x * 0.5f < -20.0f) dir.y = abs(dir.y);
}

void ScriptComponent::OnPreRender()
{
	circle = Math::Physics2D::MakeCircle(GetTransform()->localPosition, GetTransform()->scale.x * 0.5f);
	aabb = Math::Physics2D::MakeAABB(circle);
	std::list<GameObject*> objs;
	if (sp.head != nullptr)
	{
		sp.Traverse(objs, sp.head, GetGameObject());
		for (auto obj : objs)
		{
			auto script = obj->GetComponent<ScriptComponent>();
			//GameState::Get()->debugRenderer.Add_Rectangle(script->aabb, Color{ 1, 0, 0, 1 }); */
			if (Math::Physics2D::IsColliding(script->circle, circle))
			{
				GameState::Get()->debugRenderer.Add_Rectangle(script->aabb, Color{ 0, 0, 1, 1 });
				auto transform = GetTransform();
				auto unitVector = (transform->localPosition - obj->GetTransform()->localPosition).Normalize();
				if (unitVector.Magnitude() == 0)
					unitVector = Vector2(Math::RndFloat(0, 2) - 1.0f, Math::RndFloat(0, 2) - 1.0f).Normalize();
				transform->localPosition = transform->localPosition + unitVector * Utility::UTime::Get()->DeltaTimeF();
			}
		}
	}
}
