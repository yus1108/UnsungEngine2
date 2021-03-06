#pragma once
#include "UEngine.h"

namespace UEngine
{
	class Material final : public Component
	{
	private:
		const RenderObject* renderObject{ nullptr };
		DXRenderer::DXTexture* imageTexture{ nullptr };
		DXRenderer::DXConstantBuffer* colorBuffer;
		DXRenderer::DXConstantBuffer* spriteBuffer;

		void Awake() override;
		void LateUpdate() override;
		void OnPreRender() override;
		virtual void OnDestroy() override;

		virtual void SetEnable(bool enable) override {}
	public:
		Color color{ 0.5f, 0.5f, 0.5f, 1 };
		UV uv{ 0, 0, 1, 1 };
		void Load(std::wstring fileName);

	public:
		Material()
			: colorBuffer(DXRenderer::DXConstantBuffer::Instantiate
			(
				DXRenderer::Get(),
				DXRenderer::Get()->ResourceManager->GetConstantBuffer(typeid(Color).raw_name())
			)), spriteBuffer(DXRenderer::DXConstantBuffer::Instantiate
			(
				DXRenderer::Get(),
				DXRenderer::Get()->ResourceManager->GetConstantBuffer(typeid(UV).raw_name())
			))
		{}
	};
}
