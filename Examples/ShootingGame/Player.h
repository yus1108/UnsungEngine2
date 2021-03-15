#pragma once

using namespace UEngine;
class Player : public Component
{
private:
	int frame = 0;
	POINT currMapIndex{ 0, 0 };
	Material* material{ nullptr };
	Physics2D::RectCollider* collider{ nullptr };
	float hitCounter = 0;
	float hitMaxTimer = 1.0f;
	bool hitColor = false;

public:
	POINT startIndex{ 2, 0 };
	POINT endIndex{ 2, 1 };
	int maxSpriteFrame = 30;
	float movingCounter = 0.0f;
	bool reverse = false;
private:
	void Start() override;
	void FixedUpdate() override;
	void Update() override;
	void OnPreRender() override;
};

