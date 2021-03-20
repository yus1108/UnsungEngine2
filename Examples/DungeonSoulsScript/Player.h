#pragma once

using namespace UEngine;

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

struct Animation
{
	int numSprite;
	float delay;
};

Animation player_animation_map[PLAYER_ANIMATION_STATE_COUNT] =
{
	{ 13, 0.2f },
	{ 8, 0.1f },
	{ 10, 0.2f },
	{ 10, 0.2f },
	{ 10, 0.2f },
	{ 6, 0.2f },
	{ 4, 0.2f },
	{ 7, 0.2f },
	{ 4, 0.2f },
	{ 8, 0.2f },
	{ 6, 0.2f },
	{ 8, 0.2f },
	{ 5, 0.2f }
};

UENGINE_CLASS(Player)
{
private:
	bool isRolling = false;
	float frameSize = 32.0f;
	float textureSize = 416.0f;
	float index = 0;
	float timer = 0;
	unsigned animationState = PLAYER_ANIMATION_STATE_MOVE;
	float speed = 4.0f;
	Vector2 lastPos;

	Material* material{ nullptr };

public:
private:
	void Start() override;
	void Update() override;
};

