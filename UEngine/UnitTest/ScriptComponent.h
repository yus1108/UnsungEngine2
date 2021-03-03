#pragma once
#include "SpatialPartitioning.h"

using namespace UEngine;
class ScriptComponent : public Component
{
private:
	Vector2 dir;
public:
	bool isAABBColliding{ false };
	float speed{ 0 };
	float collideTimer{ 0.5f };
	float maxTimer{ 0.5f };
	Math::Physics2D::CircleCoord circle;
	Math::Physics2D::AABB aabb;
	SpatialPartitioning::Collider* collider;

	static SpatialPartitioning sp;

	void Start() override;
	void FixedUpdate() override;
	void Update() override;
	void OnPreRender() override;
};

