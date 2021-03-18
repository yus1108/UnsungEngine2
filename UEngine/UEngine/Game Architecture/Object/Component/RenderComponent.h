#pragma once

namespace UEngine
{
	class RenderComponent final : public Component
	{
	private:
		RenderObject renderObject;
		SERIALIZED_STRING(renderMesh_name);
		SERIALIZED_STRING(shader_name);

		void LateUpdate() override;
		void DeSerialize(TiXmlNode* node) override;
	public:
		void Load(std::string renderMesh_name, std::string shader_name);
		void LoadTriangle();
		void LoadRectangle();
		void LoadCircle(UINT slice = 360);
		void AddConstantBuffer(std::string type_raw_name, DXRenderer::DXConstantBuffer* buffer);
		void AddImageTexture(DXRenderer::DXTexture* imageTexture);
		void AddImageSampler(DXRenderer::DXSamplerState* imageSampler);
		const RenderObject* const GetRenderObject() { return &renderObject; }
	};
}
