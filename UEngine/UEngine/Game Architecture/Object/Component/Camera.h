#pragma once

namespace UEngine
{
	class Camera : public Component
	{
	private:
		SERIALIZED_BOOL(isThisMainCamera);
		GameView gameView;
		DXRenderer::DXView* view{ nullptr };
		DXRenderer::DXConstantBuffer* cameraBuffer{ nullptr };

		CPU_CAMERA cpu_camera{ Matrix() };
		DirectX::XMVECTOR view_determinant{0};

		void OnEnable() override;
		void OnDisable() override;
		void Awake() override;
		void Update() override;
		void LateUpdate() override;
		void OnPreRender() override;
		void OnDestroy() override;
		void DeSerialize(TiXmlNode* node) override;

	public:
		SERIALIZED_VECTOR3_INIT(cameraPosition, Vector3(0, 0, 0));
		SERIALIZED_VECTOR3_INIT(cameraRotation, Vector3(0, 0, 0));

		SERIALIZED_FLOAT_INIT(viewWidth, 0);
		SERIALIZED_FLOAT_INIT(viewHeight, 0);
		SERIALIZED_FLOAT_INIT(nearZ, 0);
		SERIALIZED_FLOAT_INIT(farZ, 0);

		Camera() 
			: cameraBuffer
			(
				DXRenderer::DXConstantBuffer::Instantiate(DXRenderer::Get(),
				DXRenderer::Get()->ResourceManager->GetCBufferPreset(typeid(CPU_CAMERA).raw_name()))
			) {}

		void SetMainCamera();
		static Camera* const GetMainCamera() { return GameState::GetCurrentScene()->MainCamera; }
		CPU_CAMERA GetCameraMatrix() { return CPU_CAMERA{ DirectX::XMMatrixTranspose(cpu_camera.view), DirectX::XMMatrixTranspose(cpu_camera.projection) }; }
	};
}
