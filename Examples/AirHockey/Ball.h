#pragma once
using namespace UEngine;
class Ball :
    public Component
{
private:
    Vector2 direction = Vector2();
    float speed = 0.0f;
private:
    void Update() override;
public:
    void Init();
};

