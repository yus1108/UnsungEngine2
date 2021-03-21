#pragma once
#include "Animation.h"

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

UENGINE_CLASS(Player)
{
private:
	float frameSize = 32.0f;
	float timer = 0;
	float speed = 4.0f;
	float deltaTime = 0.0f;
	Vector2 lastPos;
	Vector2 velocity;

	Animation player_animation_map[PLAYER_ANIMATION_STATE_COUNT];
	Animation animation = player_animation_map[PLAYER_ANIMATION_STATE_IDLE];

	bool Movable = false;
	bool Rotatable = false;
	bool EnableRoutine = false;
	Transform* transform = nullptr;
	Material* material{ nullptr };
	GameObject* imageObj{ nullptr };

private:
	void Start() override;
	void Update() override;
	void LateUpdate() override;

	void RotateOn(float x);
	void ReceiveInput();
	void UpdateAnimation();
	void OnPreRender();
};

