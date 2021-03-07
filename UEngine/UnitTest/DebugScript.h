#pragma once
using namespace UEngine;
class DebugScript : public Component
{
	static int sid;
	int id = 1;
	float speed;
	Vector2 dir;
public:
	void Start();
	void Update();
	void OnPreRender();
};

