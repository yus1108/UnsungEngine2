#include "UEngine.h"
#include "RenderComponent.h"

void UEngine::RenderComponent::OnEnable()
{
	if (renderObject) GameState::Get()->gameScene.AddObject(renderObject);
	else if (!renderMesh_name.empty() && !shader_name.empty())
		Load(renderMesh_name, shader_name);
}

void UEngine::RenderComponent::OnDisable()
{
	if (renderObject)
	{
		GameState::Get()->gameScene.RemoveObject(renderObject);
		renderObject = nullptr;
	}
}

void UEngine::RenderComponent::Load(std::string renderMesh_name, std::string shader_name)
{
	OnDisable();
	renderObject = GameState::Get()->gameScene.LoadObject(renderMesh_name, shader_name);
	this->renderMesh_name = renderMesh_name;
	this->shader_name = shader_name;
	if (GetGameObject()->GetActive() && GetEnable())
	{
		GameState::Get()->gameScene.AddObject(renderObject);
	}
}

void UEngine::RenderComponent::LoadTriangle()
{
	Load("triangle", "color");
}

void UEngine::RenderComponent::LoadRectangle()
{
	Load("rectangle", "color");
}

void UEngine::RenderComponent::LoadCircle(UINT slice)
{
	if (slice == 360)
	{
		Load("circle", "color");
		return;
	}

	if (DXRenderer::DXResourceManager::Get()->GetRenderMesh("circle" + slice) == nullptr)
	{
		std::vector<DXRenderer::SIMPLE_VERTEX> vertices = std::vector<DXRenderer::SIMPLE_VERTEX>
		{
			DXRenderer::SIMPLE_VERTEX{DirectX::XMFLOAT3{0, 0, 0}}
		};
		float radian = Utility::UMath::PI * 2.0f / slice;
		for (size_t i = 0; i < slice; i++)
			vertices.emplace_back(DXRenderer::SIMPLE_VERTEX{ DirectX::XMFLOAT3{0.5f * cos(radian * (slice - i)), 0.5f * sin(radian * (slice - i)), 0} });
		vertices.shrink_to_fit();
		DXRenderer::DXResourceManager::Get()->SetVertices("circle" + slice, vertices);
		std::vector<unsigned> indices;
		for (size_t i = 0; i < slice - 1; i++)
		{
			indices.emplace_back(0);
			indices.emplace_back(i + 1);
			indices.emplace_back(i + 2);
		}
		indices.emplace_back(0);
		indices.emplace_back(slice);
		indices.emplace_back(1);
		DXRenderer::DXResourceManager::Get()->SetRenderMesh("circle" + slice, DXRenderer::DXRenderMesh::Instantiate<DXRenderer::SIMPLE_VERTEX>
			(
				DXRenderer::Get()->GetDevice(),
				&vertices[0],
				vertices.size(),
				&indices[0],
				indices.size()
				));
	}
	Load("circle" + slice, "color");
}

void UEngine::RenderComponent::AddConstantBuffer(std::string type_raw_name, DXRenderer::DXConstantBuffer* buffer)
{
	renderObject->constantBuffers[type_raw_name] = buffer;
}

void UEngine::RenderComponent::AddImageTexture(DXRenderer::DXTexture* imageTexture)
{
	renderObject->imageTexture = imageTexture;
}
