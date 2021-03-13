#pragma once

using namespace UEngine;
class Tile : public Component
{
public:
	Vector2 lastpos;
	Vector2 fixedPosition;

private:
	void Start() override;
	void FixedUpdate() override;
	void Update() override;
};

