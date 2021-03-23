#include "pch.h"
#include "Weapon.h"

void Weapon::OnTriggerEnter(Physics2D::Collider* other)
{
	if (isAttack)
	{
		auto health = other->GetComponent<Health>();
		if (health != nullptr && 
			health->GetGameObject()->GetParent() != GetGameObject()->GetParent())
		{
			if (colliders.find(other) == colliders.end())
			{
				health->GetHit(GetTransform()->localPosition.value);
				colliders.insert(other);
			}
		}
	}
}

void Weapon::OnTriggerStay(Physics2D::Collider* other)
{
	if (isAttack)
	{
		auto health = other->GetComponent<Health>();
		if (health != nullptr &&
			health->GetGameObject()->GetParent() != GetGameObject()->GetParent())
		{
			if (colliders.find(other) == colliders.end())
			{
				health->GetHit(GetTransform()->localPosition.value);
				colliders.insert(other);
			}
		}
	}
}

void Weapon::Attack()
{
	colliders.clear();
}
