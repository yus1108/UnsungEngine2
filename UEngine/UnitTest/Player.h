#pragma once

using namespace UEngine;
class Player : public Component
{
private:
	int frame = 0;
	POINT currMapIndex{ 0, 0 };
	Material* material{ nullptr };
	Physics2D::RectCollider* collider;
	Vector2 lastpos;
	float gravity = -1;
	float weight = 0;
	bool ableToJump = false;

public:
	POINT maxMapIndex{ 6, 1 };
	int maxSpriteFrame = 30;
private:
	void Start() override;
	void FixedUpdate() override;
	void Update() override;
	void OnPreRender() override;
};

