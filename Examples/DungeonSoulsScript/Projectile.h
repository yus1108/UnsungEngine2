#pragma once

UENGINE_CLASS(Projectile)
{
public:
	GameObject* parent = nullptr;
	Vector2 velocity = Vector2();
	float timer = 10;

private:
	virtual void Update() override;
	virtual void OnTriggerEnter(Physics2D::Collider * other) override;
};

