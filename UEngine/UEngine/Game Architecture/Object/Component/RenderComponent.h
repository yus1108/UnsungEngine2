#pragma once

namespace UEngine
{
	class RenderComponent final : public Component
	{
	private:
		RenderObject* renderObject;
		SERIALIZED_STRING(renderMesh_name);
		SERIALIZED_STRING(shader_name);

		void OnPreRender() override;
		void OnEnable() override { renderObject->isRenderable = true; }
		void OnDisable() override { renderObject->isRenderable = false; }
		void OnDestroy() override { renderObject->isRenderable = false; }
		void DeSerialize(TiXmlNode* node) override;
	public:
		RenderComponent() : renderObject(new RenderObject) {}
		~RenderComponent() { delete renderObject; }

		void Load(std::string renderMesh_name, std::string shader_name);
		void LoadTriangle();
		void LoadRectangle();
		void LoadCircle(UINT slice = 360);
		void AddConstantBuffer(std::string type_raw_name, DXRenderer::DXConstantBuffer* buffer);
		void AddImageTexture(DXRenderer::DXTexture* imageTexture);
		void AddImageSampler(DXRenderer::DXSamplerState* imageSampler);
		const RenderObject* const GetRenderObject() { return renderObject; }
	};
}
