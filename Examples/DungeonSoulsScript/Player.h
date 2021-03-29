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

UENGINE_CLASS(Player)
{
private:
	float frameSize = 32.0f;
	float speed = 4.0f;
	float deltaTime = 0.0f;

	bool hitRed = true;
	float hitTimer = 0.0f;
	float hitMaxTimer = 0.4f;
	Vector2 hitDirection;

	Vector2 velocity;

	float gravity = -30;
	float gVelocity = 0;
	Vector2 weight;

	Vector2 dashDisplacement;
	Vector2 hitDashDisplacement;

	Dash dash;
	Dash attackDash{ 100.0f, 0.2f, 0.0f };
	Dash jumpDash{ 500.0f, 0.2f, 0.0f };
	Dash hitDashX{ 200.0f, 0.2f, 0.0f };
	Dash hitDashY{ 50.0f, 0.1f, 0.0f };

	GameObject* tiles[40][23];

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
	bool deletionMode = false;

public:
	bool showCollision = false;
	bool EditorMode = true;

private:
	void Start() override;
	void FixedUpdate() override;
	void Update() override;
	void LateUpdate() override;
	void OnDestroy() override;

	void OnCollisionStay(Physics2D::Collider * collision) override;
	void OnCollisionEnter(Physics2D::Collider * collision) override;

	void RotateOn(float x);
	void ReceiveInput();
	void UpdateAnimation();
	void OnPreRender();
	void DashUpdate();
	void AttackInput();
	void SetAttack();

	void CreateTile();
	void CreateSkeleton();
	void CreateBoss();

public:
	void GetHit(Vector2 from);
};

