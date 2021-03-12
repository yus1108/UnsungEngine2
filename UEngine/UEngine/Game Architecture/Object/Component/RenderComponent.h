#pragma once

namespace UEngine
{
	class RenderComponent : public Component
	{
	private:
		RenderObject renderObject;
		std::wstring renderMesh_name;
		std::wstring shader_name;

		void LateUpdate() override;
	public:
		void Load(std::wstring renderMesh_name, std::wstring shader_name);
		void LoadTriangle();
		void LoadRectangle();
		void LoadCircle(UINT slice = 360);
		void AddConstantBuffer(std::string type_raw_name, DXRenderer::DXConstantBuffer* buffer);
		void AddImageTexture(DXRenderer::DXTexture* imageTexture);
		const RenderObject* const GetRenderObject() { return &renderObject; }
	};
}
