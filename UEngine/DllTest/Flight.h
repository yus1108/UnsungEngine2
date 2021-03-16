#pragma once

using namespace UEngine;
UENGINE_CLASS(Flight)
{
private:
	int frame = 0;
	POINT currMapIndex{ 0, 0 };
	Material* material{ nullptr };

public:
	POINT startIndex{ 3, 0 };
	POINT endIndex{ 7, 0 };
	int maxSpriteFrame = 30;
	float movingCounter = 0.0f;
	bool reverse = false;
private:
	void Start() override;
	void Update() override;
};

