#pragma once

class Puzzle :
    public Component
{
public:
    GameObject* pieces[3][3];
    void Update() override;
};

