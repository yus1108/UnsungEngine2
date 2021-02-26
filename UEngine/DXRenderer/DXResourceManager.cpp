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

		void DXResourceManager::SetConstantBuffer(std::string resource_name, CONSTANT_BUFFER_DESC constantBuffer)
		{
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
				true,
				rendering_desc.EnableBlendState,
				&rsDesc
			);

			shaders["color"] = DXShader::Instantiate
			(
				renderer,
				"../_Shaders/WorldVS.hlsl",
				"../_Shaders/ColorPS.hlsl",
				rendering_desc.IsDebuggable,
				true,
				rendering_desc.EnableBlendState,
				&rsDesc
			);

			rsDesc.FillMode = D3D11_FILL_WIREFRAME;
			// Create view layout
			D3D11_INPUT_ELEMENT_DESC vLayout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			shaders["debug"] = DXShader::Instantiate
			(
				renderer,
				"../_Shaders/DebugRenderVS.hlsl",
				"../_Shaders/DebugRenderPS.hlsl",
				rendering_desc.IsDebuggable,
				false,
				false,
				&rsDesc
			);
			shaders["debug"]->InitInputLayout(renderer->GetDevice(), vLayout, ARRAYSIZE(vLayout));
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

			{
				unsigned slice = 360;
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
				renderMeshes["circle"] = DXRenderMesh::Instantiate<SIMPLE_VERTEX>
					(
						device,
						&vertices[0],
						vertices.size(),
						&indices[0],
						indices.size()
						);
			}
		}

		void DXResourceManager::InitConstantBuffer()
		{
			auto renderer = DXRenderer::Get();

			constantBuffers[typeid(Color).raw_name()] = CONSTANT_BUFFER_DESC
			{
				typeid(Color).raw_name(),
				sizeof(Color),
				UENGINE_DXRENDERER_SHADERTYPE_PIXEL_SHADER,
				nullptr
			};

			constantBuffers[typeid(CPU_WORLD).raw_name()] = CONSTANT_BUFFER_DESC
			{
				typeid(CPU_WORLD).raw_name(),
				sizeof(CPU_WORLD),
				UENGINE_DXRENDERER_SHADERTYPE_VERTEX_SHADER,
				nullptr
			};

			constantBuffers[typeid(CPU_CAMERA).raw_name()] = CONSTANT_BUFFER_DESC
			{
				typeid(CPU_CAMERA).raw_name(),
				sizeof(CPU_CAMERA),
				UENGINE_DXRENDERER_SHADERTYPE_VERTEX_SHADER,
				new UINT[1] { 1 }
			};
		}
	}
}