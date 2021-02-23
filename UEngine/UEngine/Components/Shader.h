#pragma once

namespace UEngine
{
	class Shader : public IComponent
	{
	private:
		DXRenderer::DXShader* shaderBuffer;

	public:
		void Instantiate(std::string resource_name);
		virtual void OnEnable() override { gameObject()->renderObject->SetShader(shaderBuffer); }
		virtual void OnDisable() override { gameObject()->renderObject->SetShader(nullptr); }
	};

}