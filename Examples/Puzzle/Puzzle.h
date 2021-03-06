#pragma once
#include <stack>

class Puzzle :
    public Component
{
public:
    enum class Direction
    {
        left,
        top,
        right,
        bottom
    };
    GameObject* pieces[3][3]{ nullptr };
    POINT currPos{ 0,0 };
    std::stack<Direction> stack;
    bool start = false;

    void Start() override;
    void Update() override;

    bool Swap(Direction dir);
};

