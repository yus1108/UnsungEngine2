#pragma once
#include "SpatialPartitioning.h"

using namespace UEngine;
class ScriptComponent : public Component
{
private:
	Vector2 dir;
public:
	Math::Physics2D::AABB aabb;
	static SpatialPartitioning sp;
	void Start() override;
	void Update() override;
	void OnPreRender() override;
};

