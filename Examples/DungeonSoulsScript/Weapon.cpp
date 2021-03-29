#include "pch.h"
#include "Weapon.h"

void Weapon::OnTriggerEnter(Physics2D::Collider* other)
{
	if (isAttack)
	{
		auto parent = GetGameObject()->GetParent();
		auto otherParent = other->GetGameObject()->GetParent();
		if (!otherParent) return;


		if (!(parent->GetComponent<Skeleton>() && otherParent->GetComponent<Skeleton>())
			)
		{
			auto health = other->GetComponent<Health>();
			if (health != nullptr &&
				other->GetGameObject()->GetParent() != GetGameObject()->GetParent())
			{
				if (colliders.find(other) == colliders.end())
				{
					Console::WriteLine("attack");
					health->GetHit(GetTransform()->GetParent()->localPosition.value, damage);
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
		auto parent = GetGameObject()->GetParent();
		auto otherParent = other->GetGameObject()->GetParent();
		if (!otherParent) return;


		if (!(parent->GetComponent<Skeleton>() && otherParent->GetComponent<Skeleton>())
			)
		{
			auto health = other->GetComponent<Health>();
			if (health != nullptr &&
				other->GetGameObject()->GetParent() != GetGameObject()->GetParent())
			{
				if (colliders.find(other) == colliders.end())
				{
					health->GetHit(GetTransform()->GetParent()->localPosition.value, damage);
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
