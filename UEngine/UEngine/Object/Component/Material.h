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

		void Awake() override;
		void LateUpdate() override;
		void OnPreRender() override;
		virtual void OnDestroy() override;
	public:
		Color color{ 0.5f, 0.5f, 0.5f, 1 };
		void Load(std::wstring fileName);

	public:
		Material() : colorBuffer(DXRenderer::DXConstantBuffer::Instantiate
		(
			DXRenderer::Get(),
			DXRenderer::DXResourceManager::Get()->GetConstantBuffer(typeid(Color).raw_name())
		)) {}
	};
}
