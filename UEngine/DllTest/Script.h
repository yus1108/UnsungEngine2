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
	UEngine::Physics2D::AABB aabb{ 0.5f, 0.5f, 0.5f, 0.5f };
	static Script* isSelected;
public:
	TRANSFORM_MODE transformMode = TRANSFORM_MODE::TRANSLATION;
	void Update() override;
	void LateUpdate() override;
	void OnPreRender() override;
};

