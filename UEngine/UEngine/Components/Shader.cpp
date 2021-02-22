#include "UEngine.h"
#include "Shader.h"

void UEngine::Shader::Instantiate(const std::string& vertex_shader_file, const std::string& pixel_shader_file, bool isDebuggable, bool enableBlending, const DXRenderer::RASTERIZER_DESC* const rasterizerStateDesc)
{
	DXRenderer::DXShader::Instantiate
	(
		DXRenderer::Get(),
		vertex_shader_file,
		pixel_shader_file,
		isDebuggable,
		enableBlending,
		rasterizerStateDesc
	);
	gameObject()->renderObject->SetShader(shaderBuffer);
}
