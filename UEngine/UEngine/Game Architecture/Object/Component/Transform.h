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
		Matrix CameraRTP{ DirectX::XMMatrixIdentity() };

		void Awake() override;
		void Start() override;
		void Update() override;
		void LateUpdate() override;
		void OnPreRender() override;
		virtual void OnDestroy() override { GetGameObject()->GetScene()->ResourceManager.RemoveResource<DXRenderer::DXConstantBuffer>(worldBuffer->UID); }

	public:
		SERIALIZED_VECTOR3(localPosition);
		SERIALIZED_VECTOR3(localRotation);
		SERIALIZED_VECTOR3_INIT(scale, Vector3(1, 1, 1));

		const Matrix& GetRTP() { return RTP; }
		const Matrix GetWorld() { return XMMatrixTranspose(world.matrix); }

		Transform* GetParent();
		Transform* GetChild(UINT index);

		void SetParent(Transform* transform) { GetGameObject()->SetParent(transform->GetGameObject()); }
		void AddChild(Transform* transform) { GetGameObject()->AddChild(transform->GetGameObject()); }
		std::vector<Transform*> GetChildren();
	};
}
