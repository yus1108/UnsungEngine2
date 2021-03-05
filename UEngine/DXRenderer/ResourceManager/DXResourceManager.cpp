#include "dxrframework.h"
#include "DXResourceManager.h"
#include "..\Utility\UMath.h"
#include "DXScene.h"

// compiled shaders list
#ifdef _DEBUG
#include "Shader Files\Debug\DefaultVS.csh"
#include "Shader Files\Debug\DebugRenderVS.csh"
#include "Shader Files\Debug\WorldVS.csh"
#include "Shader Files\Debug\SpriteVS.csh"

#include "Shader Files\Debug\DefaultPS.csh"
#include "Shader Files\Debug\DebugRenderPS.csh"
#include "Shader Files\Debug\ColorPS.csh"
#else
#include "Shader Files\RELEASE\DefaultVS.csh"
#include "Shader Files\RELEASE\DebugRenderVS.csh"
#include "Shader Files\RELEASE\WorldVS.csh"
#include "Shader Files\RELEASE\SpriteVS.csh"

#include "Shader Files\RELEASE\DefaultPS.csh"
#include "Shader Files\RELEASE\DebugRenderPS.csh"
#include "Shader Files\RELEASE\ColorPS.csh"
#endif

namespace UEngine
{
	namespace DXRenderer
	{
		void DXResourceManager::SetShaders(std::string resource_name, DXShader* shader)
		{
			if (shaders[resource_name] != nullptr)
				DXShader::Release(&shaders[resource_name]);

			shaders[resource_name] = shader;
		}

		void DXResourceManager::SetRenderMesh(std::string resource_name, DXRenderMesh* renderMesh)
		{
			if (shared_rendermesh[resource_name]->renderMesh != nullptr)
				DXRenderMesh::Release(&shared_rendermesh[resource_name]->renderMesh);

			shared_rendermesh[resource_name]->renderMesh = renderMesh;
		}

		void DXResourceManager::SetVertices(std::string resource_name, const std::vector<SIMPLE_VERTEX>& vertices)
		{
			shared_rendermesh[resource_name]->vertices = vertices;
		}

		void DXResourceManager::SetConstantBuffer(std::string resource_name, CONSTANT_BUFFER_DESC constantBuffer)
		{
			constantBuffers[resource_name] = constantBuffer;
		}

		DXRenderMesh* DXResourceManager::GetRenderMesh(std::string resource_name)
		{
	/*		if (renderMeshes[resource_name] == nullptr)
				return loadedRenderMeshes[resource_name];*/
			return renderMeshes[resource_name];
		}

		std::vector<SIMPLE_VERTEX> DXResourceManager::GetVertices(std::string resource_name)
		{
			//if (vertexInfo[resource_name].size() == 0)
				//return loadedVertexInfo[resource_name];
			return vertexInfo[resource_name];
		}

		void DXResourceManager::Init()
		{
			Release();
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
				DefaultVS, ARRAYSIZE(DefaultVS),
				DefaultPS, ARRAYSIZE(DefaultPS),
				rendering_desc.IsDebuggable,
				true,
				rendering_desc.EnableBlendState,
				&rsDesc
			);

			shaders["image"] = DXShader::Instantiate
			(
				renderer,
				WorldVS, ARRAYSIZE(WorldVS),
				DefaultPS, ARRAYSIZE(DefaultPS),
				rendering_desc.IsDebuggable,
				true,
				rendering_desc.EnableBlendState,
				&rsDesc
			);

			shaders["sprite"] = DXShader::Instantiate
			(
				renderer,
				SpriteVS, ARRAYSIZE(SpriteVS),
				DefaultPS, ARRAYSIZE(DefaultPS),
				rendering_desc.IsDebuggable,
				true,
				rendering_desc.EnableBlendState,
				&rsDesc
			);

			shaders["color"] = DXShader::Instantiate
			(
				renderer,
				WorldVS, ARRAYSIZE(WorldVS),
				ColorPS, ARRAYSIZE(ColorPS),
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
				DebugRenderVS, ARRAYSIZE(DebugRenderVS),
				DebugRenderPS, ARRAYSIZE(DebugRenderPS),
				rendering_desc.IsDebuggable,
				false,
				false,
				&rsDesc
			);
			shaders["debug"]->InitInputLayout
			(
				renderer->GetDevice(), 
				vLayout, ARRAYSIZE(vLayout), 
				DebugRenderVS, ARRAYSIZE(DebugRenderVS)
			);
		}

		void DXResourceManager::InitRenderMesh()
		{
			auto device = DXRenderer::Get()->GetDevice();

			{
				auto vertices = std::vector<SIMPLE_VERTEX>
				{
					SIMPLE_VERTEX{DirectX::XMFLOAT3{-1, -1, 0}, DirectX::XMFLOAT2{0, 1}},
					SIMPLE_VERTEX{DirectX::XMFLOAT3{-1, 1, 0}, DirectX::XMFLOAT2{0, 0}},
					SIMPLE_VERTEX{DirectX::XMFLOAT3{1, -1, 0}, DirectX::XMFLOAT2{1, 1}},
					SIMPLE_VERTEX{DirectX::XMFLOAT3{1, 1, 0}, DirectX::XMFLOAT2{1, 0}},
				};
				vertices.shrink_to_fit();
				vertexInfo["default"] = vertices;
				unsigned indices[] = { 0, 1, 2, 2, 1, 3 };
				renderMeshes["default"] =
					DXRenderMesh::Instantiate<SIMPLE_VERTEX>
					(
						device,
						&vertices[0],
						vertices.size(),
						indices,
						ARRAYSIZE(indices)
						);
			}
			
			{
				auto vertices = std::vector<SIMPLE_VERTEX>
				{
					SIMPLE_VERTEX{DirectX::XMFLOAT3{0, 0, 0}},
				};
				vertices.shrink_to_fit();
				vertexInfo["point"] = vertices;
				renderMeshes["point"] =
					DXRenderMesh::Instantiate<SIMPLE_VERTEX>
					(
						device,
						&vertices[0],
						vertices.size(),
						D3D11_PRIMITIVE_TOPOLOGY_POINTLIST
						);
			}

			{
				auto vertices = std::vector<SIMPLE_VERTEX>
				{
					SIMPLE_VERTEX{DirectX::XMFLOAT3{0, 0, 0}},
					SIMPLE_VERTEX{DirectX::XMFLOAT3{0, 1.0f, 0}},
				};
				vertices.shrink_to_fit();
				vertexInfo["line"] = vertices;
				renderMeshes["line"] = DXRenderMesh::Instantiate<SIMPLE_VERTEX>
					(
						device,
						&vertices[0],
						vertices.size()
						);
			}

			{
				float radian = Utility::UMath::PI * 2.0f / 3.0f;
				float startRadian = Utility::UMath::PI / 2.0f;
				auto vertices = std::vector<SIMPLE_VERTEX>
				{
					SIMPLE_VERTEX{DirectX::XMFLOAT3{0.5f * cos(startRadian + radian), 0.5f * sin(startRadian + radian), 0}, DirectX::XMFLOAT2{ 0, 1 }},
					SIMPLE_VERTEX{DirectX::XMFLOAT3{0, 0.5f * sin(startRadian), 0}, DirectX::XMFLOAT2{ 0.5f, 0 }},
					SIMPLE_VERTEX{DirectX::XMFLOAT3{0.5f * cos(startRadian + radian * 2.0f), 0.5f * sin(startRadian + radian * 2.0f), 0}, DirectX::XMFLOAT2{ 1, 1 }},
				};
				vertices.shrink_to_fit();
				vertexInfo["triangle"] = vertices;
				renderMeshes["triangle"] = DXRenderMesh::Instantiate<SIMPLE_VERTEX>
					(
						device,
						&vertices[0],
						vertices.size(),
						D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
						);
			}

			{
				auto vertices = std::vector<SIMPLE_VERTEX>
				{
					SIMPLE_VERTEX{DirectX::XMFLOAT3{-0.5f, -0.5f, 0}, DirectX::XMFLOAT2{0, 1}},
					SIMPLE_VERTEX{DirectX::XMFLOAT3{-0.5f, 0.5f, 0}, DirectX::XMFLOAT2{0, 0}},
					SIMPLE_VERTEX{DirectX::XMFLOAT3{0.5f, -0.5f, 0}, DirectX::XMFLOAT2{1, 1}},
					SIMPLE_VERTEX{DirectX::XMFLOAT3{0.5f, 0.5f, 0}, DirectX::XMFLOAT2{1, 0}},
				};
				vertices.shrink_to_fit();
				vertexInfo["rectangle"] = vertices;
				unsigned indices[] = { 0, 1, 2, 2, 1, 3 };
				renderMeshes["rectangle"] = DXRenderMesh::Instantiate<SIMPLE_VERTEX>
					(
						device,
						&vertices[0],
						vertices.size(),
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
					vertices.emplace_back(SIMPLE_VERTEX{ DirectX::XMFLOAT3{0.5f * cos(radian * (slice - i)), 0.5f * sin(radian * (slice - i)), 0} });
				vertices.shrink_to_fit();
				vertexInfo["circle"] = vertices;
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

			constantBuffers[typeid(UV).raw_name()] = CONSTANT_BUFFER_DESC
			{
				typeid(UV).raw_name(),
				sizeof(UV),
				UENGINE_DXRENDERER_SHADERTYPE_VERTEX_SHADER,
				new UINT[1] { 2 }
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

		void DXResourceManager::Release()
		{
			{
				for (auto resource : shaders)
					DXShader::Release(&resource.second);
				for (auto resource : renderMeshes)
					DXRenderMesh::Release(&resource.second);
				for (auto resource : constantBuffers)
					delete resource.second.StartSlots;
				for (auto resource : shared_rendermesh)
					DXRenderMesh::Release(&resource.second->renderMesh);
				for (auto resource : scenes)
					delete resource.second;
				shared_rendermesh.clear();
				scenes.clear();
			}
		}
	}
}