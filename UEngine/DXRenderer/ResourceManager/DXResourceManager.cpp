#include "dxrframework.h"
#include "DXResourceManager.h"
#include "../Utility/UMath.h"

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
		void DXResourceManager::Init()
		{
			DXResourceManager::TYPE_VIEW = typeid(UEngine::DXRenderer::DXView).raw_name();
			DXResourceManager::TYPE_TEXTURE = typeid(UEngine::DXRenderer::DXTexture).raw_name();
			DXResourceManager::TYPE_SHADER = typeid(UEngine::DXRenderer::DXShader).raw_name();
			DXResourceManager::TYPE_RENDERMESH = typeid(UEngine::DXRenderer::DXRenderMesh).raw_name();
			DXResourceManager::TYPE_CONSTANT_BUFFER = typeid(UEngine::DXRenderer::DXConstantBuffer).raw_name();
			InitShaders();
			InitMeshes();
			InitCBuffers();
		}

		void DXResourceManager::Release()
		{
			ApplyChange();
			for (auto map : resources)
			{
				std::string resourceType = map.first;
				auto list = map.second;

				for (auto resource : list)
				{
					if (resourceType == TYPE_VIEW)
						DXView::Release(static_cast<DXView*>(resource.second));
					if (resourceType == TYPE_TEXTURE)
						DXTexture::Release(static_cast<DXTexture*>(resource.second));
					if (resourceType == TYPE_SHADER)
						DXShader::Release(static_cast<DXShader*>(resource.second));
					if (resourceType == TYPE_RENDERMESH)
						DXRenderMesh::Release(static_cast<DXRenderMesh*>(resource.second));
					if (resourceType == TYPE_CONSTANT_BUFFER)
						DXConstantBuffer::Release(static_cast<DXConstantBuffer*>(resource.second));
				}
			}
			resources.clear();
			for (auto desc : cBufferPreset)
				delete desc.second.StartSlots;
			cBufferPreset.clear();
		}

		void DXResourceManager::InitShaders()
		{
			auto renderer = DXRenderer::Get();
			auto rendering_desc = renderer->GetDescription();

			RASTERIZER_DESC rsDesc = RASTERIZER_DESC();
			rsDesc.EnableAntialisedLine = rendering_desc.EnableAntialisedLine;
			rsDesc.EnableDepthStencil = rendering_desc.EnableDepthStencil;
			rsDesc.EnableMultisampling = rendering_desc.EnableMultisampling;

			resources[TYPE_SHADER]["default"] = DXShader::Instantiate
			(
				renderer,
				DefaultVS, ARRAYSIZE(DefaultVS),
				DefaultPS, ARRAYSIZE(DefaultPS),
				rendering_desc.IsDebuggable,
				true,
				rendering_desc.EnableBlendState,
				&rsDesc
			);

			resources[TYPE_SHADER]["image"] = DXShader::Instantiate
			(
				renderer,
				WorldVS, ARRAYSIZE(WorldVS),
				DefaultPS, ARRAYSIZE(DefaultPS),
				rendering_desc.IsDebuggable,
				true,
				rendering_desc.EnableBlendState,
				&rsDesc
			);

			resources[TYPE_SHADER]["sprite"] = DXShader::Instantiate
			(
				renderer,
				SpriteVS, ARRAYSIZE(SpriteVS),
				DefaultPS, ARRAYSIZE(DefaultPS),
				rendering_desc.IsDebuggable,
				true,
				rendering_desc.EnableBlendState,
				&rsDesc
			);

			resources[TYPE_SHADER]["color"] = DXShader::Instantiate
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
			resources[TYPE_SHADER]["debug"] = DXShader::Instantiate
			(
				renderer,
				DebugRenderVS, ARRAYSIZE(DebugRenderVS),
				DebugRenderPS, ARRAYSIZE(DebugRenderPS),
				rendering_desc.IsDebuggable,
				false,
				false,
				&rsDesc
			);
			static_cast<DXShader*>(resources[TYPE_SHADER]["debug"])->InitInputLayout
			(
				renderer->GetDevice(),
				vLayout, ARRAYSIZE(vLayout),
				DebugRenderVS, ARRAYSIZE(DebugRenderVS)
			);
		}

		void DXResourceManager::InitMeshes()
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
				std::vector<unsigned> indices = { 0, 1, 2, 2, 1, 3 };
				resources[TYPE_RENDERMESH]["default"] =
					DXRenderMesh::Instantiate
					(
						device,
						vertices,
						indices
						);
			}

			{
				auto vertices = std::vector<SIMPLE_VERTEX>
				{
					SIMPLE_VERTEX{DirectX::XMFLOAT3{0, 0, 0}},
				};
				vertices.shrink_to_fit();
				resources[TYPE_RENDERMESH]["point"] =
					DXRenderMesh::Instantiate
					(
						device,
						vertices,
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
				resources[TYPE_RENDERMESH]["line"] = DXRenderMesh::Instantiate
					(
						device,
						vertices
						);
			}

			{
				float radian = UEngine::Utility::UMath::PI * 2.0f / 3.0f;
				float startRadian = UEngine::Utility::UMath::PI / 2.0f;
				auto vertices = std::vector<SIMPLE_VERTEX>
				{
					SIMPLE_VERTEX{DirectX::XMFLOAT3{0.5f * cos(startRadian + radian), 0.5f * sin(startRadian + radian), 0}, DirectX::XMFLOAT2{ 0, 1 }},
					SIMPLE_VERTEX{DirectX::XMFLOAT3{0, 0.5f * sin(startRadian), 0}, DirectX::XMFLOAT2{ 0.5f, 0 }},
					SIMPLE_VERTEX{DirectX::XMFLOAT3{0.5f * cos(startRadian + radian * 2.0f), 0.5f * sin(startRadian + radian * 2.0f), 0}, DirectX::XMFLOAT2{ 1, 1 }},
				};
				vertices.shrink_to_fit();
				resources[TYPE_RENDERMESH]["triangle"] = DXRenderMesh::Instantiate
					(
						device,
						vertices,
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
				std::vector<unsigned> indices = { 0, 1, 2, 2, 1, 3 };
				resources[TYPE_RENDERMESH]["rectangle"] = DXRenderMesh::Instantiate
					(
						device,
						vertices,
						indices
						);
			}

			{
				unsigned slice = 360;
				std::vector<SIMPLE_VERTEX> vertices = std::vector<SIMPLE_VERTEX>
				{
					SIMPLE_VERTEX{DirectX::XMFLOAT3{0, 0, 0}}
				};
				float radian = UEngine::Utility::UMath::PI * 2.0f / slice;
				for (size_t i = 0; i < slice; i++)
					vertices.emplace_back(SIMPLE_VERTEX{ DirectX::XMFLOAT3{0.5f * cos(radian * (slice - i)), 0.5f * sin(radian * (slice - i)), 0} });
				vertices.shrink_to_fit();
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
				resources[TYPE_RENDERMESH]["circle"] = DXRenderMesh::Instantiate
					(
						device,
						vertices,
						indices
						);
			}
		}

		void DXResourceManager::InitCBuffers()
		{
			auto renderer = DXRenderer::Get();

			cBufferPreset[typeid(Color).raw_name()] = CONSTANT_BUFFER_DESC
			{
				typeid(Color).raw_name(),
				sizeof(Color),
				UENGINE_DXRENDERER_SHADERTYPE_PIXEL_SHADER,
				nullptr
			};

			cBufferPreset[typeid(UV).raw_name()] = CONSTANT_BUFFER_DESC
			{
				typeid(UV).raw_name(),
				sizeof(UV),
				UENGINE_DXRENDERER_SHADERTYPE_VERTEX_SHADER,
				new UINT[1] { 2 }
			};

			cBufferPreset[typeid(CPU_WORLD).raw_name()] = CONSTANT_BUFFER_DESC
			{
				typeid(CPU_WORLD).raw_name(),
				sizeof(CPU_WORLD),
				UENGINE_DXRENDERER_SHADERTYPE_VERTEX_SHADER,
				nullptr
			};

			cBufferPreset[typeid(CPU_CAMERA).raw_name()] = CONSTANT_BUFFER_DESC
			{
				typeid(CPU_CAMERA).raw_name(),
				sizeof(CPU_CAMERA),
				UENGINE_DXRENDERER_SHADERTYPE_VERTEX_SHADER,
				new UINT[1] { 1 }
			};
		}

		void DXResourceManager::ApplyChange()
		{
			using namespace UEngine::DXRenderer;
			for (auto map : creationQueue)
			{
				std::string resourceType = map.first;
				auto list = map.second;

				for (auto resource : list)
				{
					if (resources[resourceType].size() > 0 &&
						resources[resourceType].find(resource.first) != resources[resourceType].end())
						throw std::runtime_error("A resource with the given name already exists");

					resources[resourceType][resource.first] = resource.second;
				}
			}
			creationQueue.clear();

			for (auto map : deletionQueue)
			{
				std::string resourceType = map.first;
				auto list = map.second;

				for (auto resource : list)
				{
					if (resources[resourceType].size() == 0 &&
						resources[resourceType].find(resource.first) == resources[resourceType].end())
						throw std::runtime_error("A resource with the given name doesn't exists");

					resources[resourceType].erase(resource.first);

					if (resourceType == TYPE_VIEW)
						DXView::Release(static_cast<DXView*>(resource.second));
					if (resourceType == TYPE_TEXTURE)
						DXTexture::Release(static_cast<DXTexture*>(resource.second));
					if (resourceType == TYPE_SHADER)
						DXShader::Release(static_cast<DXShader*>(resource.second));
					if (resourceType == TYPE_RENDERMESH)
						DXRenderMesh::Release(static_cast<DXRenderMesh*>(resource.second));
					if (resourceType == TYPE_CONSTANT_BUFFER)
						DXConstantBuffer::Release(static_cast<DXConstantBuffer*>(resource.second));
				}
			}
			deletionQueue.clear();
		}

	}
}