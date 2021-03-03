#pragma once
#include "dxrframework.h"

namespace UEngine
{
	namespace DXRenderer
	{
		class DXShader final
		{
		private:
			DXShader() = default;
			~DXShader() = default;
		private:
			Pipeline pipeline;
			std::string shader_files[5];

			void InitInputLayout(bool enableInitInputLayout, ID3D11Device* const device, const void* VShaderByteCode, SIZE_T VShaderLength);

		public:
			void InitInputLayout(ID3D11Device* const device, D3D11_INPUT_ELEMENT_DESC* vLayout, SIZE_T vLayoutLength, const void* VShaderByteCode, SIZE_T VShaderLength);
			void SetShader(bool enableInitInputLayout, ID3D11Device* const device, const void* shaderByteCode, SIZE_T shaderLength, const UENGINE_DXRENDERER_SHADERTYPE& shader_type);
			void SetShader(bool enableInitInputLayout, ID3D11Device* const device, ID3DBlob* const shaderBlob, const UENGINE_DXRENDERER_SHADERTYPE& shader_type);
			void SetShader(bool enableInitInputLayout, ID3D11Device* const device, const std::string& shader_file, const UENGINE_DXRENDERER_SHADERTYPE& shader_type,
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
				bool enableInitInputLayout,
				bool enableBlending,
				const RASTERIZER_DESC* const rasterizerStateDesc
			);
			static DXShader* Instantiate
			(
				DXRenderer* const renderer,
				const void* pVShaderByteCode, SIZE_T VShaderLength,
				const void* pPShaderByteCode, SIZE_T PShaderLength,
				bool isDebuggable,
				bool enableInitInputLayout,
				bool enableBlending,
				const RASTERIZER_DESC* const rasterizerStateDesc
			);
			static void Release(DXShader** const shader);
			void Set(ID3D11DeviceContext* const deviceContext);
		};
	}
}
