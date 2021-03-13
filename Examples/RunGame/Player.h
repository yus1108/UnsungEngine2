#pragma once

using namespace UEngine;
class Player : public Component
{
private:
	int frame = 0;
	POINT currMapIndex{ 0, 0 };
	Material* material{ nullptr };
	Physics2D::RectCollider* collider;
	float gravity = -10.0f;
	float weight = 0;
	bool ableToJump = false;
	float hitCounter = 0;
	float hitMaxTimer = 1.0f;
	bool hitColor = false;

public:
	POINT maxMapIndex{ 6, 1 };
	int maxSpriteFrame = 30;
private:
	void Start() override;
	void FixedUpdate() override;
	void Update() override;
	void OnPreRender() override;
};

