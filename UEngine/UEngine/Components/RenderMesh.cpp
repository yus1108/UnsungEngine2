#include "UEngine.h"
#include "RenderMesh.h"

void UEngine::RenderMesh::Instantiate(const std::vector<DXRenderer::SIMPLE_VERTEX>& vertices, const std::vector<unsigned>& indices, D3D11_PRIMITIVE_TOPOLOGY topology)
{
	this->vertices = vertices;
	this->indices = indices;
	renderMeshBuffer = DXRenderer::DXRenderMesh::Instantiate
	(
		DXRenderer::Get()->GetDevice(),
		&vertices[0],
		vertices.size(),
		&indices[0],
		indices.size(),
		topology
	);
	gameObject()->renderObject->SetRenderMesh(renderMeshBuffer);
}
