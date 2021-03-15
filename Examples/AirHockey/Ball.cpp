#include "stdafx.h"
#include "Ball.h"

using namespace Math::Physics2D;
using namespace Utility;

void Ball::Update()
{
	auto transform = GetTransform();
	auto ballCol = MakeCircle(transform->localPosition.value, transform->scale.value.x * 0.5f);

	auto p1 = FindObjectWithName("p1");
	auto p1Col = MakeCircle(p1->GetTransform()->localPosition.value, p1->GetTransform()->scale.value.x * 0.5f);

	auto p2 = FindObjectWithName("p2");
	auto p2Col = MakeCircle(p2->GetTransform()->localPosition.value, p2->GetTransform()->scale.value.x * 0.5f);

	if (IsColliding(ballCol, p1Col))
	{
		Vector2 displacement = transform->localPosition.value - p1->GetTransform()->localPosition.value;
		float totRadius = transform->scale.value.x * 0.5f + p1->GetTransform()->scale.value.x * 0.5f;
		float value = totRadius - displacement.Magnitude();
		direction = displacement.Normalize();
		speed = 15.0f;
		value /= 2.0f;
		transform->localPosition.value = transform->localPosition.value + direction * value;
		p1->GetTransform()->localPosition.value = p1->GetTransform()->localPosition.value + direction * -value;
	}

	if (IsColliding(ballCol, p2Col))
	{
		Vector2 displacement = transform->localPosition.value - p2->GetTransform()->localPosition.value;
		float totRadius = transform->scale.value.x * 0.5f + p2->GetTransform()->scale.value.x * 0.5f;
		float value = totRadius - displacement.Magnitude();
		direction = displacement.Normalize();
		speed = 15.0f;
		value /= 2.0f;
		transform->localPosition.value = transform->localPosition.value + direction * value;
		p2->GetTransform()->localPosition.value = p2->GetTransform()->localPosition.value + direction * -value;
	}

	if (transform->localPosition.value.x + transform->scale.value.x * 0.5f > 7.5f)
	{
		direction.x = -abs(direction.x);
		speed *= 0.95f;
	}
	if (transform->localPosition.value.x - transform->scale.value.x * 0.5f < -7.5f)
	{
		direction.x = abs(direction.x);
		speed *= 0.95f;
	}
	if (transform->localPosition.value.y + transform->scale.value.x * 0.5f > 15.0f)
	{
		direction.y = -abs(direction.y);
		speed *= 0.95f;
	}
	if (transform->localPosition.value.y - transform->scale.value.x * 0.5f < -15.0f)
	{
		direction.y = abs(direction.y);
		speed *= 0.95f;
	}

	transform->localPosition.value = transform->localPosition.value + direction * speed * UTime::Get()->DeltaTimeF();
}

void Ball::Init()
{
	auto transform = GetTransform();
	transform->localPosition.value = Vector2();
	direction = Vector2();
	speed = 0.0f;
}
