#include "UEngine.h"
#include "RenderComponent.h"

void UEngine::RenderComponent::OnPreRender()
{
	if (!renderObject->isRenderable) return;
	auto scene = GetGameObject()->GetScene();
	// TODO: Calculate whether to draw or not
	for (size_t i = 0; i < scene->gpu_view.size(); i++)
	{
		if (!scene->gpu_view[i].isRenderable) continue;
		scene->gpu_view[i].renderObjects.emplace_back(renderObject);
	}
}

void UEngine::RenderComponent::DeSerialize(TiXmlNode* node)
{
	Serializer::DeSerialize(node);
	Load(renderMesh_name.value, shader_name.value);
}

void UEngine::RenderComponent::OnEnable()
{
	if (renderObject->shader && renderObject->renderMesh)
		renderObject->isRenderable = true;
}

void UEngine::RenderComponent::Load(std::string renderMesh_name, std::string shader_name)
{
	auto scene = GetGameObject()->GetScene();
	this->renderMesh_name = renderMesh_name;
	this->shader_name = shader_name;

	renderObject->shader = scene->ResourceManager.GetResource<DXRenderer::DXShader>(shader_name);
	if (renderObject->shader == nullptr)
	{
	}
	renderObject->renderMesh = scene->ResourceManager.GetResource<DXRenderer::DXRenderMesh>(renderMesh_name);
	if (renderObject->renderMesh == nullptr)
	{
		// load mesh file
	}
	if (GetEnable()) renderObject->isRenderable = true;
}

void UEngine::RenderComponent::LoadTriangle()
{
	Load("triangle", "color");
}

void UEngine::RenderComponent::OnEditRender()
{
	Component::OnEditRender();
}

void UEngine::RenderComponent::LoadRectangle()
{
	Load("rectangle", "color");
}

void UEngine::RenderComponent::LoadCircle(UINT slice)
{
	Load("circle", "color");
	/*if (slice == 360)
	{
		Load(L"circle", L"color");
		return;
	}*/

	/*if (DXRenderer::Get()->ResourceManager->GetRenderMesh("circle" + slice) == nullptr)
	{
		std::vector<DXRenderer::SIMPLE_VERTEX> vertices = std::vector<DXRenderer::SIMPLE_VERTEX>
		{
			DXRenderer::SIMPLE_VERTEX{DirectX::XMFLOAT3{0, 0, 0}}
		};
		float radian = Utility::UMath::PI * 2.0f / slice;
		for (size_t i = 0; i < slice; i++)
			vertices.emplace_back(DXRenderer::SIMPLE_VERTEX{ DirectX::XMFLOAT3{0.5f * cos(radian * (slice - i)), 0.5f * sin(radian * (slice - i)), 0} });
		vertices.shrink_to_fit();
		DXRenderer::Get()->ResourceManager->SetVertices("circle" + slice, vertices);
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
		DXRenderer::Get()->ResourceManager->SetRenderMesh("circle" + slice, DXRenderer::DXRenderMesh::Instantiate<DXRenderer::SIMPLE_VERTEX>
			(
				DXRenderer::Get()->GetDevice(),
				&vertices[0],
				vertices.size(),
				&indices[0],
				indices.size()
				));
	}
	Load("circle" + slice, "color");*/
}

void UEngine::RenderComponent::AddConstantBuffer(std::string type_raw_name, DXRenderer::DXConstantBuffer* buffer)
{
	renderObject->constantBuffers[type_raw_name] = buffer;
}

void UEngine::RenderComponent::AddImageTexture(DXRenderer::DXTexture* imageTexture)
{
	renderObject->textures[0] = imageTexture;
}

void UEngine::RenderComponent::AddImageSampler(DXRenderer::DXSamplerState* imageSampler)
{
	renderObject->samplerState[0] = imageSampler;
}
