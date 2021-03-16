#pragma once

using namespace UEngine;
UENGINE_CLASS(Boost)
{
private:
	Material* material{ nullptr };

public:
	float frameSize = 32.0f;
	float textureSize = 352.0f;
	float index = 0;
	float timer = 0;
	float delay = 0.05f;
private:
	void Start() override;
	void Update() override;
};

