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
		Material() : colorBuffer(DXRenderer::DXResourceManager::Get()->GetConstantBuffer("color")) { colorBuffer->AttachData(&color, sizeof(Color)); }
		virtual void OnEnable() override { gameObject()->GetRenderObject()->AddConstantBuffer("color", colorBuffer); }
		virtual void OnDisable() override { gameObject()->GetRenderObject()->RemoveConstantBuffer("color"); }
	};
}
