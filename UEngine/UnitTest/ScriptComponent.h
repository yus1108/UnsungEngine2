#pragma once

using namespace UEngine;
class ScriptComponent : public Component
{
public:
	Vector2 dir;
	float speed{ 0 };
	float collideTimer{ 0.5f };
	float maxTimer{ 0.5f };
	Math::Physics2D::CircleCoord circle;
	Math::Physics2D::AABB aabb;

	void Start() override;
	void FixedUpdate() override;
	void Update() override;
	void OnPreRender() override;
};

