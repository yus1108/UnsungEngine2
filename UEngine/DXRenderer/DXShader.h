#pragma once
#include "dxrframework.h"

namespace UEngine
{
	class DXShader final
	{
	private:
		DXShader() {}
		~DXShader() = default;
	private:
		ID3D11Device* device{ nullptr };
		ID3D11DeviceContext* deviceContext{ nullptr };
		DXRPipeline pipeline;

		std::string shader_files[static_cast<unsigned>(ShaderType::COUNT)];
		Microsoft::WRL::ComPtr<ID3DBlob> vsShaderBlob = nullptr;

	public:
		void SetInputLayout(ID3D11InputLayout* const input_layout) { pipeline.inputLayout = input_layout; }

		void SetShader(ID3D11VertexShader* const shader) { pipeline.vertexShader = shader; }
		void SetShader(ID3D11PixelShader* const shader) { pipeline.pixelShader = shader; }
		void SetShader(ID3D11GeometryShader* const shader) { pipeline.geometryShader = shader; }
		void SetShader(ID3D11HullShader* const shader) { pipeline.hullShader = shader; }
		void SetShader(ID3D11DomainShader* const shader) { pipeline.domainShader = shader; }
		void SetShader(ID3DBlob* const shaderBlob, const ShaderType& shader_type);
		void SetShader(const std::string& shader_file, const ShaderType& shader_type);
		void SetShader(const std::string& shader_file, const std::string& entry_point, const ShaderType& shader_type);
		void SetShaders(const std::string shader_files[static_cast<unsigned>(ShaderType::COUNT)]);
		void InitInputLayout();

		static HRESULT CompileShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ ID3D11Device* device,
			_Outptr_ ID3DBlob** blob, LPCSTR shader_version, bool isDebuggable = false);

		static DXShader* Init(ID3D11Device* const device, ID3D11DeviceContext* const deviceContext);
		static void Release(DXShader** const shader);
		void Render();

	};
}
