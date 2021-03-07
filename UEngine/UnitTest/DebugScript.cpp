#include "framework.h"
#include "DebugScript.h"

int DebugScript::sid = 0;

void DebugScript::Start()
{
	id = ++sid;

	auto x = (float)rand() / (rand() % INT_MAX);
	auto y = (float)rand() / (rand() % INT_MAX);
	speed = UEngine::Math::RndFloat(1.0f, 5.0f);
	float scale = UEngine::Math::RndFloat(1.0f, 3.0f);
	GetTransform()->scale = Vector3(scale, scale, 1);
	GetComponent<CircleCollider>()->SetCollider(GetTransform()->localPosition, scale / 2.0f);
	dir.x = x;
	dir.y = y;
	dir = dir.Normalize();
	dir = dir;
}

void DebugScript::OnPreRender()
{
	GameState* gameState = GameState::Get();
	auto sp = gameState->GetSpatialPartition2D();
	if (id == sid)
	{
		//sp->DebugRender(sp->head, GetComponent<CircleCollider>(), Color{ 1, 0, 0, 1 }, Color{ 0, 0, 1, 1 });
	}
	//sp->DebugRender(sp->head, GetComponent<CircleCollider>(), Color{ 1, 1, 0, 1 });
}

void DebugScript::Update()
{
	using namespace std;

	auto transform = GetTransform();
	auto value = dir * speed * Utility::UTime::Get()->DeltaTimeF();
	transform->localPosition = transform->localPosition + value;

	if (transform->localPosition.x + transform->scale.x * 0.5f > Camera::mainCamera->viewWidth / 2.0f) dir.x = -abs(dir.x);
	if (transform->localPosition.x - transform->scale.x * 0.5f < -(Camera::mainCamera->viewWidth / 2.0f)) dir.x = abs(dir.x);
	if (transform->localPosition.y + transform->scale.x * 0.5f > Camera::mainCamera->viewHeight / 2.0f) dir.y = -abs(dir.y);
	if (transform->localPosition.y - transform->scale.x * 0.5f < -(Camera::mainCamera->viewHeight / 2.0f)) dir.y = abs(dir.y);
}