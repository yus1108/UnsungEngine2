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
			auto resourceManager = DXResourceManager::Get();

			DXRenderObject* renderObj = DXRenderObject::Instantiate(resourceManager->GetRenderMesh("point"), resourceManager->GetShaders("color"));
			renderObj->AddConstantBuffer("color", resourceManager->GetConstantBuffer("color"));
			DirectX::XMFLOAT4 color{ 0.5f,0.5f, 0.5f, 1 };
			renderObj->CBCopyData<DirectX::XMFLOAT4>("color", &color, sizeof(color));
			return renderObj;
		}

		DXRenderObject* DXGeometryFigurePrefab::CreateLine()
		{
			DXRenderer* renderer = DXRenderer::Get();
			auto resourceManager = DXResourceManager::Get();

			DXRenderObject* renderObj = DXRenderObject::Instantiate(resourceManager->GetRenderMesh("line"), resourceManager->GetShaders("color"));
			renderObj->AddConstantBuffer("color", resourceManager->GetConstantBuffer("color"));
			DirectX::XMFLOAT4 color{ 0.5f,0.5f, 0.5f, 1 };
			renderObj->CBCopyData<DirectX::XMFLOAT4>("color", &color, sizeof(color));
			return renderObj;
		}

		DXRenderObject* DXGeometryFigurePrefab::CreateTriangle()
		{
			DXRenderer* renderer = DXRenderer::Get();
			auto resourceManager = DXResourceManager::Get();

			DXRenderObject* renderObj = DXRenderObject::Instantiate(resourceManager->GetRenderMesh("triangle"), resourceManager->GetShaders("color"));
			renderObj->AddConstantBuffer("color", resourceManager->GetConstantBuffer("color"));
			DirectX::XMFLOAT4 color{ 0.5f,0.5f, 0.5f, 1 };
			renderObj->CBCopyData<DirectX::XMFLOAT4>("color", &color, sizeof(color));
			return renderObj;
		}

		DXRenderObject* DXGeometryFigurePrefab::CreateRectangle()
		{
			DXRenderer* renderer = DXRenderer::Get();
			auto resourceManager = DXResourceManager::Get();

			DXRenderObject* renderObj = DXRenderObject::Instantiate(resourceManager->GetRenderMesh("rectangle"), resourceManager->GetShaders("color"));
			renderObj->AddConstantBuffer("color", resourceManager->GetConstantBuffer("color"));
			DirectX::XMFLOAT4 color{ 0.5f,0.5f, 0.5f, 1 };
			renderObj->CBCopyData<DirectX::XMFLOAT4>("color", &color, sizeof(color));
			return renderObj;
		}

		DXRenderObject* DXGeometryFigurePrefab::CreateCircle(unsigned slice)
		{
			DXRenderer* renderer = DXRenderer::Get();
			auto resourceManager = DXResourceManager::Get();

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
			resourceManager->SetRenderMesh("circle_" + slice, DXRenderMesh::Instantiate<SIMPLE_VERTEX>
				(
					renderer->GetDevice(),
					&vertices[0],
					vertices.size(),
					&indices[0],
					indices.size()
					));

			DXRenderObject* renderObj = DXRenderObject::Instantiate(resourceManager->GetRenderMesh("circle_"+slice), resourceManager->GetShaders("color"));
			renderObj->AddConstantBuffer("color", resourceManager->GetConstantBuffer("color"));
			DirectX::XMFLOAT4 color{ 0.5f,0.5f, 0.5f, 1 };
			renderObj->CBCopyData<DirectX::XMFLOAT4>("color", &color, sizeof(color));
			return renderObj;
		}

	}
}
