#include "pch.h"
#include "Health.h"

void Health::GetHit(Vector2 from, float damage)
{
	auto enemy = GetGameObject()->GetParent()->GetComponent<Skeleton>();
	HP -= damage * Utility::UTime::Get()->DeltaTimeF();
	if (HP <= 0)
	{
		Dead = true;
		HP = 0;
	}
	else
	{
		if (enemy != nullptr)
		{
			enemy->GetHit(from);
		}
		else
		{
			auto player = GetGameObject()->GetParent()->GetComponent<Player>();
			if (player != nullptr)
			{
				player->GetHit(from);
			}
		}
	}
}
