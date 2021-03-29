#include "pch.h"
#include "Health.h"

void Health::GetHit(Vector2 from, float damage)
{
	auto enemy = GetGameObject()->GetParent()->GetComponent<Skeleton>();
	HP -= damage * 0.01f;
	SoundManager::Get()->PlayAudio(5, L"Assets/SPLAT Crush 01.wav", AudioType_Effects);
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
			auto boss = GetGameObject()->GetParent()->GetComponent<Boss>();
			if (boss != nullptr)
			{
				boss->GetHit(from);
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
}
