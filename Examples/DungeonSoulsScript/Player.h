#pragma once

enum PLAYER_ANIMATION_STATE
{
	PLAYER_ANIMATION_STATE_IDLE,
	PLAYER_ANIMATION_STATE_MOVE,
	PLAYER_ANIMATION_STATE_ATTACK1,
	PLAYER_ANIMATION_STATE_ATTACK2,
	PLAYER_ANIMATION_STATE_ATTACK3,
	PLAYER_ANIMATION_STATE_JUMP,
	PLAYER_ANIMATION_STATE_DAMAGED,
	PLAYER_ANIMATION_STATE_DEAD,
	PLAYER_ANIMATION_STATE_CLIMB,
	PLAYER_ANIMATION_STATE_ARROW_ATTACK,
	PLAYER_ANIMATION_STATE_MAGIC,
	PLAYER_ANIMATION_STATE_UP_RUNNING,
	PLAYER_ANIMATION_STATE_ROLL,
	PLAYER_ANIMATION_STATE_COUNT
};

enum PLAYER_DASH_ACTION
{
	PLAYER_DASH_ACTION_ATTACK,
	PLAYER_DASH_ACTION_ROLL,
	PLAYER_DASH_ACTION_NONE
};

UENGINE_CLASS(Player)
{
private:
	float frameSize = 32.0f;
	float speed = 4.0f;
	float deltaTime = 0.0f;

	bool hitRed = true;
	float hitTimer = 0.0f;
	float hitMaxTimer = 0.4f;
	Vector2 hitPower;
	Vector2 hitDirection;

	Vector2 velocity;

	float gravity = -20;
	float gVelocity = 0;
	float jumpPower = 500;
	float jumpTimer = 0;
	float jumpDuration = 0.2f;
	Vector2 weight;

	PLAYER_DASH_ACTION dashAction = PLAYER_DASH_ACTION_NONE;
	Vector2 dashDisplacement;

	float attackDash = 200;
	float attackDashValue = 0;
	float attackDashTimer = 0;
	float attackDashDuration = 0.2f;

	float dashPower = 500;
	float dashValue = 0;
	float dashTimer = 0;
	float dashDuration = 0.2f;
	float dashCooldownTimer = 0;
	float dashCooldown = 0.5f;


	Animation player_animation_map[PLAYER_ANIMATION_STATE_COUNT];
	Animation animation = player_animation_map[PLAYER_ANIMATION_STATE_IDLE];

	bool IsHit = false;
	bool Movable = false;
	bool Rotatable = false;
	bool EnableRoutine = false;
	bool Jumpable = true;
	Transform* transform = nullptr;
	Material* material = nullptr;
	class Weapon* weapon = nullptr;
	class Health* health = nullptr;
	GameObject* bodyObj = nullptr;

	GameObject* creation = nullptr;

public:
	bool showCollision = false;
	bool EditorMode = false;

private:
	void Start() override;
	void FixedUpdate() override;
	void Update() override;
	void LateUpdate() override;

	void OnCollisionStay(Physics2D::Collider * collision) override;
	void OnCollisionEnter(Physics2D::Collider * collision) override;
	void OnCollisionExit(Physics2D::Collider * collision) override;

	void RotateOn(float x);
	void ReceiveInput();
	void UpdateAnimation();
	void OnPreRender();
	void Dash(float value);
	void DashUp(float value);
	void AttackInput();
	void SetAttack();

public:
	void GetHit(Vector2 from);
};

