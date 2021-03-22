#pragma once

UENGINE_CLASS(Enemy)
{
private:
	float deltaTime = 0.0f;
	Vector2 gravity = Vector2(0, -9.81f);
	Vector2 velocity;

	bool hitRed = true;
	float hitTimer = 0.0f;
	float hitMaxTimer = 0.2f;
	Vector2 hitPower;
	Vector2 hitDirection;

	Transform* transform = nullptr;
	Material* material = nullptr;
	Physics2D::CircleCollider* collider = nullptr;

	void Start() override;
	void FixedUpdate() override;
	void Update() override;
	void LateUpdate() override;

	void OnTriggerEnter(Physics2D::Collider* other) override;
	void OnTriggerStay(Physics2D::Collider* other) override;
	void OnTriggerExit(Physics2D::Collider* other) override;

public:
	void GetHit(Vector2 from);
};

