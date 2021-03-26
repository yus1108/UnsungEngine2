#pragma once
#include "pch.h"

class SoundManager
{
public:
	static AudioEffects* Get()
	{
		if (instance == nullptr) instance = new AudioEffects();
		return instance;
	}
	static void Release() { delete instance; instance = nullptr; }

private:
	inline static AudioEffects* instance = nullptr;
	SoundManager() = default;
	~SoundManager() = default;
};