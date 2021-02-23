#include "UEngine.h"
#include "RenderMesh.h"

void UEngine::RenderMesh::Load(std::string resource_name)
{
	renderMeshBuffer = DXRenderer::DXResourceManager::Get()->GetRenderMesh(resource_name);
	if (GetEnable()) gameObject()->renderObject->SetRenderMesh(renderMeshBuffer);
}
