#include "pch.h"
#include "Projectile.h"

void Projectile::Update()
{
	GetTransform()->localPosition = GetTransform()->localPosition.value + velocity * Utility::UTime::Get()->DeltaTimeF();
	if (timer > 0)
	{
		timer -= Utility::UTime::Get()->DeltaTimeF();
	}
	else
	{
		auto obj = GetGameObject();
		obj->GetScene()->RemoveGameObject(&obj);
	}
}

void Projectile::OnTriggerEnter(Physics2D::Collider* other)
{
	if (other->GetGameObject()->name == "barrel") return;
	

	auto health = other->GetComponent<Health>();
	if (health != nullptr &&
		other->GetGameObject() != parent)
	{
		health->GetHit(Vector2(), 10);
		auto obj = GetGameObject();
		obj->GetScene()->RemoveGameObject(&obj);
		return;
	}

	if (other->GetGameObject()->name == "tile")
	{
		auto obj = GetGameObject();
		obj->GetScene()->RemoveGameObject(&obj);
		return;
	}
}
