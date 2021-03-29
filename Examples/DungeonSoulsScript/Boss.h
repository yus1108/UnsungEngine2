#pragma once

enum BOSS_ANIMATION_STATE
{
	BOSS_ANIMATION_STATE_IDLE,
	BOSS_ANIMATION_STATE_MOVE,
	BOSS_ANIMATION_STATE_ANGRY,
	BOSS_ANIMATION_STATE_ATTACK1, // basic attack
	BOSS_ANIMATION_STATE_ATTACK2, // long range attack (shooter)
	BOSS_ANIMATION_STATE_ATTACK3, // barrels attack
	BOSS_ANIMATION_STATE_ATTACK4, // close-combat ranged attack
	BOSS_ANIMATION_STATE_IDLE2,
	BOSS_ANIMATION_STATE_DAMAGED,
	BOSS_ANIMATION_STATE_DEAD,
	BOSS_ANIMATION_STATE_COUNT = 20
};


UENGINE_CLASS_INHERIT(Boss, Enemy)
{
	Animation boss_animation_map[BOSS_ANIMATION_STATE_COUNT];
	Animation animation = boss_animation_map[BOSS_ANIMATION_STATE_IDLE];
	float moveDuration = 1.0f;

	void BossUpdate();

	virtual void Start() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;

	void CreateBarrel();
public:
	int attackType = 0;
	float moveTimer = 0;
	bool delayAttack = false;

	virtual void GetHit(Vector2 from) override;

};

