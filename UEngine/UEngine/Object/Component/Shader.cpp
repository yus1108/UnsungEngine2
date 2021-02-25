#include "UEngine.h"
#include "Shader.h"

void UEngine::Shader::Load(std::string resource_name)
{
	shaderBuffer = DXRenderer::DXResourceManager::Get()->GetShaders(resource_name);
	if (GetEnable()) gameObject()->renderObject->SetShader(shaderBuffer);
}