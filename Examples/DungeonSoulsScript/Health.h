#pragma once

UENGINE_CLASS(Health)
{
private:
	float HP = 1;
	float showHP = 100;
public:
	bool Dead = false;
	void GetHit(Vector2 from, float damage);
	float GetHP() { return HP * showHP; }
	void SetHP(float hp) { showHP /= hp; HP = hp; }
};

