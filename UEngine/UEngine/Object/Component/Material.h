#pragma once
#include "UEngine.h"

namespace UEngine
{
	class Material final : public IComponent
	{
	private:
		DXRenderer::DXConstantBuffer* colorBuffer;
	public:
		Color color{ 0.5f, 0.5f, 0.5f, 1 };
	public:
		Material() : colorBuffer(DXRenderer::DXConstantBuffer::Instantiate
		(
			DXRenderer::Get(),
			DXRenderer::DXResourceManager::Get()->GetConstantBuffer(typeid(Color).raw_name())
		)) 
		{ 
			colorBuffer->AttachData(&color, sizeof(Color));
			GameState::Get()->constantBufferPool.Add(colorBuffer);
		}
		~Material() { GameState::Get()->constantBufferPool.Remove(colorBuffer); }
	};
}
