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
		std::string shader_files[5];

		void InitInputLayout(ID3D11Device* const device, ID3DBlob* const vsShaderBlob);

	public:
		void SetInputLayout(ID3D11InputLayout* const input_layout) { pipeline.inputLayout = input_layout; }

		void SetShader(ID3D11Device* const device, ID3DBlob* const shaderBlob, const UENGINE_DXSHADERTYPE& shader_type);
		void SetShader(ID3D11Device* const device, const std::string& shader_file, const UENGINE_DXSHADERTYPE& shader_type,
			bool isDebuggable, const std::string& entry_point = "main");

		static HRESULT CompileShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ ID3D11Device* device,
			_Outptr_ ID3DBlob** blob, LPCSTR shader_version, bool isDebuggable);
		
		// init DXShader with minimum requirements (Setting VS and PS with entrypoint = "main")
		static DXShader* Instantiate
		(
			DXRenderer* const renderer,
			const std::string& vertex_shader_file,
			const std::string& pixel_shader_file,
			bool isDebuggable,
			bool enableBlending,
			const DXRasterDesc* const rasterizerStateDesc
		);
		static void Release(DXShader** const shader);
		void Set(ID3D11DeviceContext* const deviceContext);
	};
}
