#pragma once

namespace UEngine
{
	class Transform final : public Component
	{
	private:
		const RenderObject* renderObject{ nullptr };
		DXRenderer::DXConstantBuffer* worldBuffer;
		Vector3 worldPosition{ 0, 0, 0 };
		CPU_WORLD world{ DirectX::XMMatrixIdentity() };
		Matrix RTP{ DirectX::XMMatrixIdentity() };

		void Awake() override;
		void LateUpdate() override;
		void OnPreRender() override;
		virtual void OnDestroy() override { GameState::Get()->constantBufferPool.Remove(worldBuffer); }

	public:
		Vector3 localPosition{ 0, 0, 0 };
		Vector3 localRotation{ 0, 0, 0 };
		Vector3 scale{ 1, 1, 1 };

		const Matrix& GetRTP() { return RTP; }
		const Matrix GetWorld() { return XMMatrixTranspose(world.matrix); }

		Transform* GetParent();
		Transform* GetChild(UINT index);

		void SetParent(Transform* transform) { GetGameObject()->SetParent(transform->GetGameObject()); }
		void AddChild(Transform* transform) { GetGameObject()->AddChild(transform->GetGameObject()); }
		std::vector<Transform*> GetChildren();
		void RemoveChild(Transform* transform) { GetGameObject()->RemoveChild(transform->GetGameObject()); }
	public:
		Transform() : worldBuffer(DXRenderer::DXConstantBuffer::Instantiate
		(
			DXRenderer::Get(),
			DXRenderer::DXResourceManager::Get()->GetConstantBuffer(typeid(CPU_WORLD).raw_name())
		)) {}
	};
}
