#pragma once

namespace UEngine
{
	class Shader : public IComponent
	{
	private:
		DXRenderer::DXShader* shaderBuffer;

	public:
		// init DXShader with minimum requirements (Setting VS and PS with entrypoint = "main")
		void Instantiate
		(
			const std::string& vertex_shader_file,
			const std::string& pixel_shader_file,
			bool isDebuggable,
			bool enableBlending,
			const DXRenderer::RASTERIZER_DESC* const rasterizerStateDesc
		);

		virtual void OnDestroy() override { DXRenderer::DXShader::Release(&shaderBuffer); }
	};

}