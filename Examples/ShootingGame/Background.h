#pragma once

using namespace UEngine;
class Background : public Component
{
private:
	float timer = 0;
	Material* material{ nullptr };

public:
	float speed = 1;
	float mapSize = 1.0f;
private:
	void Start() override;
	void Update() override;
};

