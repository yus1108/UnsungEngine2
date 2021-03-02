#pragma once
#include "SpatialPartitioning.h"

using namespace UEngine;
class ScriptComponent : public Component
{
private:
	Vector2 dir;
public:
	bool isAABBColliding{ false };
	bool isColliding{ false };
	float speed{ 0 };
	Math::Physics2D::CircleCoord circle;
	Math::Physics2D::AABB aabb;

	static SpatialPartitioning sp;

	void Start() override;
	void FixedUpdate() override;
	void Update() override;
	void OnPreRender() override;
};

