#include "dxrframework.h"
#include "DXShader.h"

void UEngine::DXShader::SetShader(ID3DBlob* const shaderBlob, const ShaderType& shader_type)
{
	switch (shader_type)
	{
	case ShaderType::VERTEX_SHADER:
		device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, pipeline.vertexShader.GetAddressOf());
		break;
	case ShaderType::PIXEL_SHADER:
		device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, pipeline.pixelShader.GetAddressOf());
		break;
	case ShaderType::GEOMETRY_SHADER:
		device->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, pipeline.geometryShader.GetAddressOf());
		break;
	case ShaderType::HULL_SHADER:
		device->CreateHullShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, pipeline.hullShader.GetAddressOf());
		break;
	case ShaderType::DOMAIN_SHADER:
		device->CreateDomainShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, pipeline.domainShader.GetAddressOf());
		break;
	default:
		assert(false && "Invalid Shader Type");
		break;
	}
}

void UEngine::DXShader::SetShader(const std::string& shader_file, const ShaderType& shader_type)
{
	SetShader(shader_file, "main", shader_type);
}

void UEngine::DXShader::SetShader(const std::string& shader_file, const std::string& entry_point, const ShaderType& shader_type)
{
	Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob = nullptr;
	shader_files[static_cast<unsigned>(shader_type)] = shader_file;
	HRESULT hr = S_OK;

	std::wstring wstr_shader_file = std::wstring(shader_file.begin(), shader_file.end());
	switch (shader_type)
	{
	case ShaderType::VERTEX_SHADER:
		hr = CompileShader(wstr_shader_file.c_str(), entry_point.c_str(), device, shaderBlob.GetAddressOf(), "vs_4_0");
		vsShaderBlob = shaderBlob;
		break;
	case ShaderType::PIXEL_SHADER:
		hr = CompileShader(wstr_shader_file.c_str(), entry_point.c_str(), device, shaderBlob.GetAddressOf(), "ps_4_0");
		break;
	case ShaderType::GEOMETRY_SHADER:
		hr = CompileShader(wstr_shader_file.c_str(), entry_point.c_str(), device, shaderBlob.GetAddressOf(), "gs_4_0");
		break;
	case ShaderType::HULL_SHADER:
		hr = CompileShader(wstr_shader_file.c_str(), entry_point.c_str(), device, shaderBlob.GetAddressOf(), "hs_4_0");
		break;
	case ShaderType::DOMAIN_SHADER:
		hr = CompileShader(wstr_shader_file.c_str(), entry_point.c_str(), device, shaderBlob.GetAddressOf(), "ds_4_0");
		break;
	default:
		assert(false && "Invalid Shader Type");
		break;
	}

	assert(hr == S_OK && "Failed to compile shader");
	SetShader(shaderBlob.Get(), shader_type);
}

void UEngine::DXShader::SetShaders(const std::string shader_files[static_cast<unsigned>(ShaderType::COUNT)])
{
	for (unsigned i = 0; i < static_cast<unsigned>(ShaderType::COUNT); i++)
	{
		if (shader_files[i].empty() == false) SetShader(shader_files[i], static_cast<ShaderType>(i));
	}
}

void UEngine::DXShader::InitInputLayout()
{
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

HRESULT UEngine::DXShader::CompileShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ ID3D11Device* device, _Outptr_ ID3DBlob** blob, LPCSTR shader_version, bool isDebuggable)
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

UEngine::DXShader* UEngine::DXShader::Init(ID3D11Device* const device, ID3D11DeviceContext* const deviceContext)
{
	UEngine::DXShader* instnace = new UEngine::DXShader;
	instnace->device = device;
	instnace->deviceContext = deviceContext;
	return instnace;
}

void UEngine::DXShader::Release(DXShader** const shader)
{
	delete *shader;
	*shader = nullptr;
}

void UEngine::DXShader::Render()
{
	deviceContext->IASetInputLayout(pipeline.inputLayout.Get());

	deviceContext->VSSetShader(pipeline.vertexShader.Get(), 0, 0);

	deviceContext->GSSetShader(pipeline.geometryShader.Get(), 0, 0);

	deviceContext->HSSetShader(pipeline.hullShader.Get(), 0, 0);

	deviceContext->DSSetShader(pipeline.domainShader.Get(), 0, 0);

	deviceContext->RSSetState(pipeline.rasterizerState.Get());

	ID3D11SamplerState* sampler[]{ pipeline.samplerState.Get() };
	deviceContext->PSSetSamplers(0, 1, sampler);
	deviceContext->PSSetShader(pipeline.pixelShader.Get(), 0, 0);

	deviceContext->OMSetBlendState(pipeline.blendingState.Get(), NULL, 0xffffffff);
}