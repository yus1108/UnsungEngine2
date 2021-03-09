#pragma once

using namespace UEngine;
class Tile : public Component
{
public:
	Vector2 lastpos;

private:
	void FixedUpdate() override;
	void Update() override;
};

