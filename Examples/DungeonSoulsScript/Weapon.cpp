#include "pch.h"
#include "Weapon.h"

void Weapon::OnTriggerEnter(Physics2D::Collider* other)
{
	if (isAttack)
	{
		auto enemy = other->GetComponent<Enemy>();
		if (enemy != nullptr)
		{
			if (colliders.find(other) == colliders.end())
			{
				enemy->GetHit(GetTransform()->localPosition.value);
				colliders.insert(other);
			}
		}
		else
		{
			auto player = other->GetComponent<Player>();
			if (player != nullptr)
			{
				if (colliders.find(other) == colliders.end())
				{
					colliders.insert(other);
				}
			}
		}
	}
}

void Weapon::OnTriggerStay(Physics2D::Collider* other)
{
	if (isAttack)
	{
		auto enemy = other->GetComponent<Enemy>();
		if (enemy != nullptr)
		{
			if (colliders.find(other) == colliders.end())
			{
				enemy->GetHit(GetTransform()->localPosition.value);
				colliders.insert(other);
			}
		}
		else
		{
			auto player = other->GetComponent<Player>();
			if (player != nullptr)
			{
				if (colliders.find(other) == colliders.end())
				{
					colliders.insert(other);
				}
			}
		}
	}
}

void Weapon::Attack()
{
	colliders.clear();
}
