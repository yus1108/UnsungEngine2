#pragma once

using namespace UEngine;
class ScriptComponent : public Component
{
private:
	bool isSelected{ false };
public:
	void Update() override;
	void OnPreRender() override;
};

