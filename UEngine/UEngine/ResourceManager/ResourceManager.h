#pragma once
#include "ConstantBufferPool.h"
#include "RenderObjectPool.h"

namespace UEngine
{
	class ResourceManager
	{
		GameState* const parent_gameState;
	public:
		ResourceManager(GameState* parent) : parent_gameState(parent) {}

		ConstantBufferPool ConstantBufferPool{ this };

		RenderObjectPool renderObjectPool{ this };
	};
}