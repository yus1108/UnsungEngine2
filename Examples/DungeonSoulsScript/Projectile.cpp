#include "pch.h"
#include "Projectile.h"

void Projectile::Update()
{
	GetTransform()->localPosition = GetTransform()->localPosition.value + velocity * Utility::UTime::Get()->DeltaTimeF();
}

void Projectile::OnTriggerEnter(Physics2D::Collider* other)
{
	auto health = other->GetComponent<Health>();
	if (health != nullptr &&
		other->GetGameObject() != parent)
	{
		health->GetHit(GetTransform()->GetParent()->localPosition.value, 10);
	}

	auto obj = GetGameObject();
	obj->GetScene()->RemoveGameObject(&obj);
}
