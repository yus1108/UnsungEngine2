#pragma once

namespace UEngine
{
	class RenderComponent : public IComponent
	{
	private:
		RenderObject* renderObject{ nullptr };

		void Awake() override { renderObject = nullptr; }
		void OnEnable() override { if (renderObject) GameState::Get()->gameScene.AddObject(renderObject); }
		void OnPreRender() override;
		void OnDisable() override { if (renderObject) GameState::Get()->gameScene.RemoveObject(renderObject); }
	public:
		void Load(std::string renderMesh_name, std::string shader_name);
		void AddConstantBuffer(std::string type_raw_name, DXRenderer::DXConstantBuffer* buffer);
	};
}
