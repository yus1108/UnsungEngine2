#pragma once

UENGINE_CLASS(Weapon)
{
private:
	bool isAttack = false;
	set<Physics2D::Collider*> colliders;

	void OnTriggerEnter(Physics2D::Collider * other) override;
	void OnTriggerStay(Physics2D::Collider * other) override;
	
public:
	void Reset() { isAttack = false; };
	void Set() { isAttack = true; }
	void Attack();
};

