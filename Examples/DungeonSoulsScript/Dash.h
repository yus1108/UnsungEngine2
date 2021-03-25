#pragma once

class Dash
{
private:
	float Timer = 0;
	float CooldownTimer = 0;
	float Value = 0;

public:
	float Power = 500;
	float Duration = 0.2f;
	float Cooldown = 0.5f;

	Dash() = default;
	Dash(float Power, float Duration, float Cooldown)
		: Power(Power), Duration(Duration), Cooldown(Cooldown)
	{}

	void Update(float deltaTime, bool isDecrement)
	{
		if (CooldownTimer > 0) CooldownTimer -= deltaTime;
		if (Timer > 0)
		{
			if (isDecrement)
				Value = -Power * deltaTime;
			else
				Value = Power * deltaTime;
			Timer -= deltaTime;
		}
		else
			Value = 0;
	}
	void Activate()
	{
		Timer = Duration;
		CooldownTimer = Cooldown;
	}
	void Stop()
	{
		Timer = 0;
		float Value = 0;
	}

	float GetValue() { return Value; }
	bool IsAvailable() { return CooldownTimer <= 0; }
};