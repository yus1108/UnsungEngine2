#pragma once
#include "UEngine.h"

namespace UEngine
{
	class Material final : public Component
	{
	private:
		const RenderObject* renderObject{ nullptr };
		SERIALIZED_STRING(fileName);
		SERIALIZED_STRING(samplerName);
		DXRenderer::DXTexture* imageTexture{ nullptr };
		DXRenderer::DXSamplerState* imageSampler{ nullptr };
		DXRenderer::DXConstantBuffer* colorBuffer;
		DXRenderer::DXConstantBuffer* spriteBuffer;

		void Awake() override;
		void Update() override;
		void OnPreRender() override;
		virtual void OnDestroy() override;
		void DeSerialize(TiXmlNode* node) override;


		virtual void SetEnable(bool enable) override {}
	public:
		SERIALIZED_COLOR_INIT(color, Color({0.5f, 0.5f, 0.5f, 1.0f}));
		SERIALIZED_UV_INIT(uv, UV({ 0, 0, 1, 1 }));
		void LoadImageMaterial(std::wstring fileName);
		void LoadImageMaterial(std::wstring fileName, D3D11_TEXTURE_ADDRESS_MODE addressMode, D3D11_FILTER filter);
		void LoadImageMaterial(std::wstring fileName, D3D11_SAMPLER_DESC desc);

	public:
		Material()
			: colorBuffer(DXRenderer::DXConstantBuffer::Instantiate
			(
				DXRenderer::Get(),
				DXRenderer::Get()->ResourceManager->GetCBufferPreset(typeid(Color).raw_name())
			)), spriteBuffer(DXRenderer::DXConstantBuffer::Instantiate
			(
				DXRenderer::Get(),
				DXRenderer::Get()->ResourceManager->GetCBufferPreset(typeid(UV).raw_name())
			))
		{}
	};
}
