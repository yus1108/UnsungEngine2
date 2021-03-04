#pragma once
class Player : public UEngine::Component
{
public:
	float speed = 8.0f;
protected:
	virtual void Update() override;
};

