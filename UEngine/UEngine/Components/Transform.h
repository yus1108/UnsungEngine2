#pragma once

namespace UEngine
{
	class Transform final : public IComponent
	{
	private:
		DXRenderer::DXConstantBuffer* worldBuffer;
		Vector3 worldPosition{ 0, 0, 0 };
		DirectX::XMMATRIX worldMat;
	public:
		Vector3 localPosition{ 0, 0, 0 };
		Vector3 localRotation{ 0, 0, 0 };
		Vector3 scale{ 1, 1, 1 };
	public:
		Transform();
		virtual void OnEnable() override;
		virtual void OnDisable() override { gameObject()->GetRenderObject()->RemoveConstantBuffer("world"); }
		virtual void Update() override;
	};
}
