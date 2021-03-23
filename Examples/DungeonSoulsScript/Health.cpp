#include "pch.h"
#include "Health.h"

void Health::GetHit(Vector2 from)
{
	auto enemy = GetGameObject()->GetParent()->GetComponent<Enemy>();
	if (enemy != nullptr)
	{
		enemy->GetHit(GetTransform()->localPosition.value);
	}
	else
	{
		auto player = GetGameObject()->GetParent()->GetComponent<Player>();
		if (player != nullptr)
		{
		}
	}
}
