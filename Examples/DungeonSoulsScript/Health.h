#pragma once

UENGINE_CLASS(Health)
{
public:
	bool Dead = false;
	float HP = 1;
	void GetHit(Vector2 from, float damage);
};

