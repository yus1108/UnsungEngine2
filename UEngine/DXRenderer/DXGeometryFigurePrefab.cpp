#include "dxrframework.h"
#include "../Utility/UMath.h"
#include "DXGeometryFigurePrefab.h"

namespace UEngine
{
	namespace DXRenderer
	{
		DXRenderObject* DXGeometryFigurePrefab::CreatePoint()
		{
			DXRenderer* renderer = DXRenderer::Get();

			// RenderMesh
			SIMPLE_VERTEX vertices[] =
			{
				SIMPLE_VERTEX{DirectX::XMFLOAT3{0, 0, 0}},
			};
			auto default_renderMesh = DXRenderMesh::Instantiate<SIMPLE_VERTEX>
				(
					renderer->GetDevice(),
					vertices,
					ARRAYSIZE(vertices),
					D3D11_PRIMITIVE_TOPOLOGY_POINTLIST
					);

			DXRenderObject* renderObj = DXRenderObject::Instantiate(default_renderMesh, renderer->GetDefaultColorShader());
			DXConstantBuffer* constantBuffer = DXConstantBuffer::Instantiate(renderer, sizeof(DirectX::XMFLOAT4), UENGINE_DXSHADERTYPE_PIXEL_SHADER);
			renderObj->AddConstantBuffer("Color", constantBuffer);
			DirectX::XMFLOAT4 color{ 0.5f,0.5f, 0.5f, 1 };
			renderObj->CBCopyData<DirectX::XMFLOAT4>("Color", &color, sizeof(color));
			return renderObj;
		}

		DXRenderObject* DXGeometryFigurePrefab::CreateLine()
		{
			DXRenderer* renderer = DXRenderer::Get();

			// RenderMesh
			SIMPLE_VERTEX vertices[] =
			{
				SIMPLE_VERTEX{DirectX::XMFLOAT3{0, 0, 0}},
				SIMPLE_VERTEX{DirectX::XMFLOAT3{0, 1.0f, 0}},
			};
			auto default_renderMesh = DXRenderMesh::Instantiate<SIMPLE_VERTEX>
				(
					renderer->GetDevice(),
					vertices,
					ARRAYSIZE(vertices)
					);

			DXRenderObject* renderObj = DXRenderObject::Instantiate(default_renderMesh, renderer->GetDefaultColorShader());
			DXConstantBuffer* constantBuffer = DXConstantBuffer::Instantiate(renderer, sizeof(DirectX::XMFLOAT4), UENGINE_DXSHADERTYPE_PIXEL_SHADER);
			renderObj->AddConstantBuffer("Color", constantBuffer);
			DirectX::XMFLOAT4 color{ 0.5f,0.5f, 0.5f, 1 };
			renderObj->CBCopyData<DirectX::XMFLOAT4>("Color", &color, sizeof(color));
			return renderObj;
		}

		DXRenderObject* DXGeometryFigurePrefab::CreateTriangle()
		{
			DXRenderer* renderer = DXRenderer::Get();

			// RenderMesh
			SIMPLE_VERTEX vertices[] =
			{
				SIMPLE_VERTEX{DirectX::XMFLOAT3{-0.5f, -0.5f, 0}},
				SIMPLE_VERTEX{DirectX::XMFLOAT3{0, 0.5f, 0}},
				SIMPLE_VERTEX{DirectX::XMFLOAT3{0.5f, -0.5f, 0}},
			};
			auto default_renderMesh = DXRenderMesh::Instantiate<SIMPLE_VERTEX>
				(
					renderer->GetDevice(),
					vertices,
					ARRAYSIZE(vertices),
					D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
					);

			DXRenderObject* renderObj = DXRenderObject::Instantiate(default_renderMesh, renderer->GetDefaultColorShader());
			DXConstantBuffer* constantBuffer = DXConstantBuffer::Instantiate(renderer, sizeof(DirectX::XMFLOAT4), UENGINE_DXSHADERTYPE_PIXEL_SHADER);
			renderObj->AddConstantBuffer("Color", constantBuffer);
			DirectX::XMFLOAT4 color{ 0.5f,0.5f, 0.5f, 1 };
			renderObj->CBCopyData<DirectX::XMFLOAT4>("Color", &color, sizeof(color));
			return renderObj;
		}

		DXRenderObject* DXGeometryFigurePrefab::CreateRectangle()
		{
			DXRenderer* renderer = DXRenderer::Get();

			// RenderMesh
			SIMPLE_VERTEX vertices[] =
			{
				SIMPLE_VERTEX{DirectX::XMFLOAT3{-0.5f, -0.5f, 0}},
				SIMPLE_VERTEX{DirectX::XMFLOAT3{-0.5f, 0.5f, 0}},
				SIMPLE_VERTEX{DirectX::XMFLOAT3{0.5f, -0.5f, 0}},
				SIMPLE_VERTEX{DirectX::XMFLOAT3{0.5f, 0.5f, 0}},
			};
			unsigned indices[] = { 0, 1, 2, 2, 1, 3 };
			auto default_renderMesh = DXRenderMesh::Instantiate<SIMPLE_VERTEX>
				(
					renderer->GetDevice(),
					vertices,
					ARRAYSIZE(vertices),
					indices,
					ARRAYSIZE(indices)
					);

			DXRenderObject* renderObj = DXRenderObject::Instantiate(default_renderMesh, renderer->GetDefaultColorShader());
			DXConstantBuffer* constantBuffer = DXConstantBuffer::Instantiate(renderer, sizeof(DirectX::XMFLOAT4), UENGINE_DXSHADERTYPE_PIXEL_SHADER);
			renderObj->AddConstantBuffer("Color", constantBuffer);
			DirectX::XMFLOAT4 color{ 0.5f,0.5f, 0.5f, 1 };
			renderObj->CBCopyData<DirectX::XMFLOAT4>("Color", &color, sizeof(color));
			return renderObj;
		}

		DXRenderObject* DXGeometryFigurePrefab::CreateCircle(unsigned slice)
		{
			DXRenderer* renderer = DXRenderer::Get();

			// RenderMesh
			std::vector<SIMPLE_VERTEX> vertices = std::vector<SIMPLE_VERTEX>
			{
				SIMPLE_VERTEX{DirectX::XMFLOAT3{0, 0, 0}}
			};
			float radian = Utility::UMath::PI * 2.0f / slice;
			for (size_t i = 0; i < slice; i++)
				vertices.emplace_back(SIMPLE_VERTEX{ DirectX::XMFLOAT3{cos(radian * (slice - i)), sin(radian * (slice - i)), 0} });
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
			auto default_renderMesh = DXRenderMesh::Instantiate<SIMPLE_VERTEX>
				(
					renderer->GetDevice(),
					&vertices[0],
					vertices.size(),
					&indices[0],
					indices.size()
					);

			DXRenderObject* renderObj = DXRenderObject::Instantiate(default_renderMesh, renderer->GetDefaultColorShader());
			DXConstantBuffer* constantBuffer = DXConstantBuffer::Instantiate(renderer, sizeof(DirectX::XMFLOAT4), UENGINE_DXSHADERTYPE_PIXEL_SHADER);
			renderObj->AddConstantBuffer("Color", constantBuffer);
			DirectX::XMFLOAT4 color{ 0.5f,0.5f, 0.5f, 1 };
			renderObj->CBCopyData<DirectX::XMFLOAT4>("Color", &color, sizeof(color));
			return renderObj;
		}

	}
}
