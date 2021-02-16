#pragma once
#include "dxrframework.h"

namespace UEngine
{
	class DXShader final
	{
		// only DXRenderer can instantiate this
		friend class UEngine::DXRenderer;
	private:
		DXShader() = default;
		~DXShader() = default;
	private:
		ID3D11Device* device;
		ID3D11DeviceContext* deviceContext;
		UEngine::DXRPipeline pipeline;

		std::string shader_files[static_cast<unsigned>(UEngine::ShaderType::COUNT)];
		Microsoft::WRL::ComPtr<ID3DBlob> vsShaderBlob = nullptr;

	public:
		void SetInputLayout(ID3D11InputLayout* const input_layout) { pipeline.inputLayout = input_layout; }

		void SetShader(ID3D11VertexShader* const shader) { pipeline.vertexShader = shader; }
		void SetShader(ID3D11PixelShader* const shader) { pipeline.pixelShader = shader; }
		void SetShader(ID3D11GeometryShader* const shader) { pipeline.geometryShader = shader; }
		void SetShader(ID3D11HullShader* const shader) { pipeline.hullShader = shader; }
		void SetShader(ID3D11DomainShader* const shader) { pipeline.domainShader = shader; }
		void SetShader(ID3DBlob* const shaderBlob, const UEngine::ShaderType& shader_type);
		void SetShader(const std::string& shader_file, const UEngine::ShaderType& shader_type);
		void SetShader(const std::string& shader_file, const std::string& entry_point, const UEngine::ShaderType& shader_type);
		void SetShaders(const std::string shader_files[static_cast<unsigned>(UEngine::ShaderType::COUNT)]);
		void InitInputLayout();

		static HRESULT CompileShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ ID3D11Device* device,
			_Outptr_ ID3DBlob** blob, LPCSTR shader_version, bool isDebuggable = false);

		void Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
		void Release();
		void Update();
		void Render();

	};
}
