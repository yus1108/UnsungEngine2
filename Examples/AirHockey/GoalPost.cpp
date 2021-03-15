#include "stdafx.h"
#include "GoalPost.h"

using namespace Math::Physics2D;
using namespace Utility;

void GoalPost::Update()
{
	auto transform = GetTransform();
	auto postCol = MakeAABB(transform->GetWorld());

	auto ball = FindObjectWithName("ball");
	auto ballCol = MakeCircle(ball->GetTransform()->localPosition.value, ball->GetTransform()->scale.value.x * 0.5f);

	if (IsColliding(postCol, ballCol))
	{
		ball->GetComponent<Ball>()->Init();
	}

	auto p1 = FindObjectWithName("p1");
	auto p1Col = MakeCircle(p1->GetTransform()->localPosition.value, p1->GetTransform()->scale.value.x * 0.5f);

	auto p2 = FindObjectWithName("p2");
	auto p2Col = MakeCircle(p2->GetTransform()->localPosition.value, p2->GetTransform()->scale.value.x * 0.5f);

	if (IsColliding(postCol, p1Col))
	{
		Vector2 closestPoint, dir;
		closestPoint.x = Math::Clamp(p1Col.center.x, postCol.left, postCol.right);
		closestPoint.y = Math::Clamp(p1Col.center.y, postCol.bottom, postCol.top);
		dir = p1Col.center - closestPoint;
		float distance = dir.Magnitude();
		distance = p1Col.radius - distance;
		p1->GetTransform()->localPosition.value = p1->GetTransform()->localPosition.value + dir * distance;
	}

	if (IsColliding(postCol, p2Col))
	{
		Vector2 closestPoint, dir;
		closestPoint.x = Math::Clamp(p2Col.center.x, postCol.left, postCol.right);
		closestPoint.y = Math::Clamp(p2Col.center.y, postCol.bottom, postCol.top);
		dir = p2Col.center - closestPoint;
		float distance = dir.Magnitude();
		distance = p1Col.radius - distance;
		p2->GetTransform()->localPosition.value = p2->GetTransform()->localPosition.value + dir * distance;
	}
}
