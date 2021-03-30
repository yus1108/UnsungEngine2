#pragma once

UENGINE_CLASS(Health)
{
private:
	SERIALIZED_FLOAT_INIT(HP, 1);
	SERIALIZED_FLOAT_INIT(showHP, 100);
public:
	bool Dead = false;
	void GetHit(Vector2 from, float damage);
	float GetHP() { return HP.value * showHP.value; }
	void SetHP(float hp) { showHP.value = 100.0f / hp; HP.value = hp; }
};

