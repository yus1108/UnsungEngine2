#pragma once

namespace UEngine
{
	class RenderMesh final : public IComponent
	{
	private:
		std::vector<DXRenderer::SIMPLE_VERTEX> vertices;
		std::vector<unsigned> indices;
		DXRenderer::DXRenderMesh* renderMeshBuffer{ nullptr };

	public:
		void Load(std::string resource_name);
		virtual void OnEnable() override { gameObject()->renderObject->SetRenderMesh(renderMeshBuffer); }
		virtual void OnDisable() override { gameObject()->renderObject->SetRenderMesh(nullptr); }
	};
}