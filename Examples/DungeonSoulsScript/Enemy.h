#pragma once

UENGINE_CLASS(Enemy)
{
protected:
	float deltaTime = 0.0f;
	Vector2 velocity;

	bool movable = false;

	bool hitRed = true;
	float hitTimer = 0.0f;
	float hitMaxTimer = 0.4f;
	Vector2 hitDirection;

	Dash hitDashX{ 350.0f, 0.1f, 0.0f };
	Dash hitDashY{ 100.0f, 0.05f, 0.0f };

	Transform* transform = nullptr;
	Material* material = nullptr;
	class Health* health = nullptr;
	class Weapon* weapon = nullptr;
	Physics2D::CircleCollider* collider = nullptr;

	bool attackable = false;
	float attackTimer = 0;
	float attackCooldown = 0;
	float playerFoundTimer = 0;
	Player* player = nullptr;
	Physics2D::CircleCollider* playerCollider = nullptr;
	Physics2D::AABB localSight = { -96, 32, 96, -32 };
	Physics2D::AABB sight = { 0, 0, 0, 0 };
	Physics2D::AABB localRange = { -4, 32, 4, -32 };
	Physics2D::AABB range = { 0, 0, 0, 0 };

	virtual void Start() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;

	virtual void OnTriggerEnter(Physics2D::Collider* other) override;
	virtual void OnTriggerStay(Physics2D::Collider* other) override;
	virtual void OnTriggerExit(Physics2D::Collider* other) override;

	virtual void OnPreRender();

	virtual void RotateOn(float x);

public:
	Vector2 gravity = Vector2(0, -9.81f);

	float AttackTime = 0.15f;
	float MoveSpeed = 32.0f;
	float PlayerFoundDuration = 5.0f;
	float AttackDelay = 1.0f;

	virtual void GetHit(Vector2 from);
};

