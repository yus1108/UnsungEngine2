#pragma once
#include "pch.h"

UENGINE_CLASS(Script)
{
public:
	enum class TRANSFORM_MODE
	{
		TRANSLATION,
		ROTATION,
		SCALE,
		NONE
	};
private:
	bool isMoving{ false };
	UEngine::Vector2 posDiff;
	UEngine::Physics2D::AABB aabb{ 0.5f, 0.5f, 0.5f, 0.5f };
	static Script* isSelected;
	static bool isKeyDown;
public:
	TRANSFORM_MODE transformMode = TRANSFORM_MODE::TRANSLATION;
	void Update() override;
	void LateUpdate() override;
	void OnPreRender() override;
};

