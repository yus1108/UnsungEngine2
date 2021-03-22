#pragma once

UENGINE_CLASS(Enemy)
{
private:
	float deltaTime = 0.0f;
	Vector2 gravity = Vector2(0, -9.81f);
	Vector2 velocity;

	Transform* transform = nullptr;
	Physics2D::CircleCollider* collider = nullptr;

	void Start() override;
	void Update() override;
	void LateUpdate() override;

	void OnTriggerEnter(Physics2D::Collider* other) override;
	void OnTriggerStay(Physics2D::Collider* other) override;
	void OnTriggerExit(Physics2D::Collider* other) override;
};

