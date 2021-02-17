#pragma once
#include "dxrframework.h"

namespace UEngine
{
	class DXShader final
	{
	private:
		DXShader() = default;
		~DXShader() = default;
	private:
		DXRPipeline pipeline;

		std::string shader_files[static_cast<unsigned>(ShaderType::COUNT)];

		void InitInputLayout(ID3D11Device* const device, ID3DBlob* const vsShaderBlob);

	public:
		void SetInputLayout(ID3D11InputLayout* const input_layout) { pipeline.inputLayout = input_layout; }

		void SetShader(ID3D11Device* const device, ID3DBlob* const shaderBlob, const ShaderType& shader_type);
		void SetShader(ID3D11Device* const device, const std::string& shader_file, const ShaderType& shader_type, const std::string& entry_point = "main");

		static HRESULT CompileShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ ID3D11Device* device,
			_Outptr_ ID3DBlob** blob, LPCSTR shader_version, bool isDebuggable = false);
		
		// init DXShader with minimum requirements (Setting VS and PS with entrypoint = "main")
		static DXShader* Instantiate(ID3D11Device* const device, const std::string& vertex_shader_file, const std::string& pixel_shader_file);
		static void Release(DXShader** const shader);
		void Render(ID3D11DeviceContext* const deviceContext);
	};
}
