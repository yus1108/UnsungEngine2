#include "dxrframework.h"
#include "DXResourceManager.h"
#include "..\Utility\UMath.h"

namespace UEngine
{
	namespace DXRenderer
	{
		DXResourceManager DXResourceManager::instance;

		void DXResourceManager::SetShaders(std::string resource_name, DXShader* shader)
		{
			auto resource = shaders[resource_name];
			if (resource != nullptr)
			{
				shaders.erase(resource_name);
				DXShader::Release(&resource);
			}

			shaders[resource_name] = shader;
		}

		void DXResourceManager::SetRenderMesh(std::string resource_name, DXRenderMesh* renderMesh)
		{
			auto resource = renderMeshes[resource_name];
			if (resource != nullptr)
			{
				renderMeshes.erase(resource_name);
				DXRenderMesh::Release(&resource);
			}

			renderMeshes[resource_name] = renderMesh;
		}

		void DXResourceManager::SetConstantBuffer(std::string resource_name, DXConstantBuffer* constantBuffer)
		{
			auto resource = constantBuffers[resource_name];
			if (resource != nullptr)
			{
				constantBuffers.erase(resource_name);
				DXConstantBuffer::Release(&resource);
			}

			constantBuffers[resource_name] = constantBuffer;
		}

		void DXResourceManager::Init()
		{
			InitShader();
			InitRenderMesh();
			InitConstantBuffer();
		}

		void DXResourceManager::InitShader()
		{
			auto renderer = DXRenderer::Get();
			auto rendering_desc = renderer->GetDescription();

			RASTERIZER_DESC rsDesc = RASTERIZER_DESC();
			rsDesc.EnableAntialisedLine = rendering_desc.EnableAntialisedLine;
			rsDesc.EnableDepthStencil = rendering_desc.EnableDepthStencil;
			rsDesc.EnableMultisampling = rendering_desc.EnableMultisampling;

			shaders["default"] = DXShader::Instantiate
			(
				renderer,
				"../_Shaders/DefaultVS.hlsl",
				"../_Shaders/DefaultPS.hlsl",
				rendering_desc.IsDebuggable,
				rendering_desc.EnableBlendState,
				&rsDesc
			);

			shaders["color"] = DXShader::Instantiate
			(
				renderer,
				"../_Shaders/WorldVS.hlsl",
				"../_Shaders/ColorPS.hlsl",
				rendering_desc.IsDebuggable,
				rendering_desc.EnableBlendState,
				&rsDesc
			);
		}

		void DXResourceManager::InitRenderMesh()
		{
			auto device = DXRenderer::Get()->GetDevice();

			{
				SIMPLE_VERTEX vertices[] =
				{
					SIMPLE_VERTEX{DirectX::XMFLOAT3{-1, -1, 0}, DirectX::XMFLOAT2{0, 1}},
					SIMPLE_VERTEX{DirectX::XMFLOAT3{-1, 1, 0}, DirectX::XMFLOAT2{0, 0}},
					SIMPLE_VERTEX{DirectX::XMFLOAT3{1, -1, 0}, DirectX::XMFLOAT2{1, 1}},
					SIMPLE_VERTEX{DirectX::XMFLOAT3{1, 1, 0}, DirectX::XMFLOAT2{1, 0}},
				};
				unsigned indices[] = { 0, 1, 2, 2, 1, 3 };
				renderMeshes["default"] =
					DXRenderMesh::Instantiate<SIMPLE_VERTEX>
					(
						device,
						&vertices[0],
						ARRAYSIZE(vertices),
						indices,
						ARRAYSIZE(indices)
						);
			}
			
			{
				SIMPLE_VERTEX vertices[] =
				{
					SIMPLE_VERTEX{DirectX::XMFLOAT3{0, 0, 0}},
				};
				renderMeshes["point"] =
					DXRenderMesh::Instantiate<SIMPLE_VERTEX>
					(
						device,
						vertices,
						ARRAYSIZE(vertices),
						D3D11_PRIMITIVE_TOPOLOGY_POINTLIST
						);
			}

			{
				SIMPLE_VERTEX vertices[] =
				{
					SIMPLE_VERTEX{DirectX::XMFLOAT3{0, 0, 0}},
					SIMPLE_VERTEX{DirectX::XMFLOAT3{0, 1.0f, 0}},
				};
				renderMeshes["line"] = DXRenderMesh::Instantiate<SIMPLE_VERTEX>
					(
						device,
						vertices,
						ARRAYSIZE(vertices)
						);
			}

			{
				SIMPLE_VERTEX vertices[] =
				{
					SIMPLE_VERTEX{DirectX::XMFLOAT3{-0.5f, -0.5f, 0}},
					SIMPLE_VERTEX{DirectX::XMFLOAT3{0, 0.5f, 0}},
					SIMPLE_VERTEX{DirectX::XMFLOAT3{0.5f, -0.5f, 0}},
				};
				renderMeshes["triangle"] = DXRenderMesh::Instantiate<SIMPLE_VERTEX>
					(
						device,
						vertices,
						ARRAYSIZE(vertices),
						D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
						);
			}

			{
				SIMPLE_VERTEX vertices[] =
				{
					SIMPLE_VERTEX{DirectX::XMFLOAT3{-0.5f, -0.5f, 0}},
					SIMPLE_VERTEX{DirectX::XMFLOAT3{-0.5f, 0.5f, 0}},
					SIMPLE_VERTEX{DirectX::XMFLOAT3{0.5f, -0.5f, 0}},
					SIMPLE_VERTEX{DirectX::XMFLOAT3{0.5f, 0.5f, 0}},
				};
				unsigned indices[] = { 0, 1, 2, 2, 1, 3 };
				renderMeshes["rectangle"] = DXRenderMesh::Instantiate<SIMPLE_VERTEX>
					(
						device,
						vertices,
						ARRAYSIZE(vertices),
						indices,
						ARRAYSIZE(indices)
						);
			}
		}

		void DXResourceManager::InitConstantBuffer()
		{
			auto renderer = DXRenderer::Get();

			constantBuffers["color"] =
				DXConstantBuffer::Instantiate
				(
					renderer,
					sizeof(DirectX::XMFLOAT4),
					UENGINE_DXRENDERER_SHADERTYPE_PIXEL_SHADER
				);

			constantBuffers["world"] =
				DXConstantBuffer::Instantiate
				(
					renderer,
					sizeof(DirectX::XMMATRIX),
					UENGINE_DXRENDERER_SHADERTYPE_VERTEX_SHADER
				);
		}
	}
}