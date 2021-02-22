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
		void Instantiate
		(
			const std::vector<DXRenderer::SIMPLE_VERTEX>& vertices,
			const std::vector<unsigned>& indices,
			D3D11_PRIMITIVE_TOPOLOGY topology
		);

		virtual void OnDestroy() override { DXRenderer::DXRenderMesh::Release(&renderMeshBuffer); }
	};
}