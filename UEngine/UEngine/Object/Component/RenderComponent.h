#pragma once

namespace UEngine
{
	class RenderComponent : public Component
	{
	private:
		RenderObject* renderObject{ nullptr };

		void Awake() override { renderObject = nullptr; }
		void OnEnable() override { if (renderObject) GameState::Get()->gameScene.AddObject(renderObject); }
		void OnDisable() override { if (renderObject) GameState::Get()->gameScene.RemoveObject(renderObject); }
	public:
		void Load(std::string renderMesh_name, std::string shader_name);
		void LoadTriangle();
		void LoadRectangle();
		void LoadCircle(UINT slice = 360);
		void AddConstantBuffer(std::string type_raw_name, DXRenderer::DXConstantBuffer* buffer);
		void AddImageTexture(DXRenderer::DXTexture* imageTexture);
		const RenderObject* const GetRenderObject() { return renderObject; }
	};
}
