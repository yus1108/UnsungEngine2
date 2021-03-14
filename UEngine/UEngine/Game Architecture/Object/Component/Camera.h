#pragma once

namespace UEngine
{
	class Camera : public Component
	{
	private:
		bool isThisMainCamera{ false };
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

	public:
		Vector3 cameraPosition{ 0, 0, 0 };
		Vector3 cameraRotation{ 0, 0, 0 };

		float viewWidth{ 0 };
		float viewHeight{ 0 };
		float nearZ{ 0 };
		float farZ{ 0 };

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
