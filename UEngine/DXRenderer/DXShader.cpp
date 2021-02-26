#include "dxrframework.h"
#include "DXShader.h"

namespace UEngine
{
	namespace DXRenderer
	{
		void DXShader::SetShader(bool enableInitInputLayout, ID3D11Device* const device, ID3DBlob* const shaderBlob, const UENGINE_DXRENDERER_SHADERTYPE& shader_type)
		{
			switch (shader_type)
			{
			case UENGINE_DXRENDERER_SHADERTYPE_VERTEX_SHADER:
				device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, pipeline.vertexShader.GetAddressOf());
				vsShaderBlob = shaderBlob;
				InitInputLayout(enableInitInputLayout, device, shaderBlob);
				break;
			case UENGINE_DXRENDERER_SHADERTYPE_PIXEL_SHADER:
				device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, pipeline.pixelShader.GetAddressOf());
				break;
			case UENGINE_DXRENDERER_SHADERTYPE_GEOMETRY_SHADER:
				device->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, pipeline.geometryShader.GetAddressOf());
				break;
			case UENGINE_DXRENDERER_SHADERTYPE_HULL_SHADER:
				device->CreateHullShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, pipeline.hullShader.GetAddressOf());
				break;
			case UENGINE_DXRENDERER_SHADERTYPE_DOMAIN_SHADER:
				device->CreateDomainShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, pipeline.domainShader.GetAddressOf());
				break;
			default:
				assert(false && "Invalid Shader Type");
				break;
			}
		}

		void DXShader::SetShader(bool enableInitInputLayout, ID3D11Device* const device, const std::string& shader_file, const UENGINE_DXRENDERER_SHADERTYPE& shader_type, bool isDebuggable, const std::string& entry_point)
		{
			Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob = nullptr;
			shader_files[static_cast<unsigned>(shader_type)] = shader_file;
			HRESULT hr = S_OK;

			std::wstring wstr_shader_file = std::wstring(shader_file.begin(), shader_file.end());
			switch (shader_type)
			{
			case UENGINE_DXRENDERER_SHADERTYPE_VERTEX_SHADER:
				hr = CompileShader(wstr_shader_file.c_str(), entry_point.c_str(), device, shaderBlob.GetAddressOf(), "vs_4_0", isDebuggable);
				break;
			case UENGINE_DXRENDERER_SHADERTYPE_PIXEL_SHADER:
				hr = CompileShader(wstr_shader_file.c_str(), entry_point.c_str(), device, shaderBlob.GetAddressOf(), "ps_4_0", isDebuggable);
				break;
			case UENGINE_DXRENDERER_SHADERTYPE_GEOMETRY_SHADER:
				hr = CompileShader(wstr_shader_file.c_str(), entry_point.c_str(), device, shaderBlob.GetAddressOf(), "gs_4_0", isDebuggable);
				break;
			case UENGINE_DXRENDERER_SHADERTYPE_HULL_SHADER:
				hr = CompileShader(wstr_shader_file.c_str(), entry_point.c_str(), device, shaderBlob.GetAddressOf(), "hs_4_0", isDebuggable);
				break;
			case UENGINE_DXRENDERER_SHADERTYPE_DOMAIN_SHADER:
				hr = CompileShader(wstr_shader_file.c_str(), entry_point.c_str(), device, shaderBlob.GetAddressOf(), "ds_4_0", isDebuggable);
				break;
			default:
				assert(false && "Invalid Shader Type");
				break;
			}

			assert(hr == S_OK && "Failed to compile shader");
			SetShader(enableInitInputLayout, device, shaderBlob.Get(), shader_type);
		}

		void DXShader::InitInputLayout(bool enableInitInputLayout, ID3D11Device* const device, ID3DBlob* const vsShaderBlob)
		{
			if (enableInitInputLayout == false) return;
			assert(pipeline.vertexShader.Get() != nullptr && "Vertex Shader must be set");
			// Create view layout
			D3D11_INPUT_ELEMENT_DESC vLayout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};

			device->CreateInputLayout(vLayout, ARRAYSIZE(vLayout), vsShaderBlob->GetBufferPointer(), vsShaderBlob->GetBufferSize(), pipeline.inputLayout.GetAddressOf());
		}

		void DXShader::InitInputLayout(ID3D11Device* const device, D3D11_INPUT_ELEMENT_DESC* inputLayout, UINT numElements)
		{
			assert(pipeline.vertexShader.Get() != nullptr && "Vertex Shader must be set");
			device->CreateInputLayout(inputLayout, numElements, vsShaderBlob->GetBufferPointer(), vsShaderBlob->GetBufferSize(), pipeline.inputLayout.GetAddressOf());
		}

		HRESULT DXShader::CompileShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ ID3D11Device* device, _Outptr_ ID3DBlob** blob, LPCSTR shader_version, bool isDebuggable)
		{
			if (!srcFile || !entryPoint || !device || !blob)
				return E_INVALIDARG;

			*blob = nullptr;

			UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

			if (isDebuggable) flags |= D3DCOMPILE_DEBUG;

			const D3D_SHADER_MACRO defines[] =
			{
				NULL, NULL
			};

			ID3DBlob* shaderBlob = nullptr;
			ID3DBlob* errorBlob = nullptr;
			HRESULT hr = D3DCompileFromFile(srcFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
				entryPoint, shader_version,
				flags, 0, &shaderBlob, &errorBlob);
			if (FAILED(hr))
			{
				if (errorBlob)
				{
					OutputDebugStringA((char*)errorBlob->GetBufferPointer());
					errorBlob->Release();
				}

				if (shaderBlob)
					shaderBlob->Release();

				return hr;
			}

			*blob = shaderBlob;

			return hr;
		}

		DXShader* DXShader::Instantiate
		(
			DXRenderer* const renderer,
			const std::string& vertex_shader_file,
			const std::string& pixel_shader_file,
			bool isDebuggable,
			bool enableInitInputLayout,
			bool enableBlending,
			const RASTERIZER_DESC* const rasterizerStateDesc
		)
		{
			auto device = renderer->GetDevice();

			DXShader* instance = new DXShader;
			instance->SetShader(enableInitInputLayout, device, vertex_shader_file, UENGINE_DXRENDERER_SHADERTYPE_VERTEX_SHADER, isDebuggable);
			instance->SetShader(false, device, pixel_shader_file, UENGINE_DXRENDERER_SHADERTYPE_PIXEL_SHADER, isDebuggable);

			// Rasterizer State
			renderer->InitRasterizerState
			(
				instance->pipeline.rasterizerState.GetAddressOf(),
				rasterizerStateDesc->FillMode,
				rasterizerStateDesc->CullMode,
				rasterizerStateDesc->EnableMultisampling,
				rasterizerStateDesc->EnableAntialisedLine,
				rasterizerStateDesc->EnableDepthStencil
			);

			// Sampler State
			D3D11_SAMPLER_DESC samplerDesc;
			ZeroMemory(&samplerDesc, sizeof(samplerDesc));
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.MaxAnisotropy = 1;
			samplerDesc.MaxLOD = FLT_MAX;
			samplerDesc.MinLOD = -FLT_MAX;
			samplerDesc.MipLODBias = 0;
			device->CreateSamplerState(&samplerDesc, &instance->pipeline.samplerState);

			// Blending State
			D3D11_BLEND_DESC blendDesc;
			ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
			blendDesc.AlphaToCoverageEnable = true;
			blendDesc.IndependentBlendEnable = false;
			blendDesc.RenderTarget[0].BlendEnable = true;
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
			blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			if (enableBlending) device->CreateBlendState(&blendDesc, &instance->pipeline.blendingState);

			return instance;
		}

		void DXShader::Release(DXShader** const shader)
		{
			delete* shader;
			*shader = nullptr;
		}

		void DXShader::Set(ID3D11DeviceContext* const deviceContext)
		{
			deviceContext->IASetInputLayout(pipeline.inputLayout.Get());

			deviceContext->VSSetShader(pipeline.vertexShader.Get(), 0, 0);

			deviceContext->GSSetShader(pipeline.geometryShader.Get(), 0, 0);

			deviceContext->HSSetShader(pipeline.hullShader.Get(), 0, 0);

			deviceContext->DSSetShader(pipeline.domainShader.Get(), 0, 0);

			deviceContext->RSSetState(pipeline.rasterizerState.Get());

			deviceContext->PSSetSamplers(0, 1, pipeline.samplerState.GetAddressOf());
			deviceContext->PSSetShader(pipeline.pixelShader.Get(), 0, 0);

			deviceContext->OMSetBlendState(pipeline.blendingState.Get(), NULL, 0xffffffff);
		}
	}
}