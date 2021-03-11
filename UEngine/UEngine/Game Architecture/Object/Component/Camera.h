#pragma once

namespace UEngine
{
	class Camera : public Component
	{
		friend class GameView;
	private:
		GameView* gameView{ nullptr };
		DXRenderer::DXView* view{ nullptr };
		DXRenderer::DXConstantBuffer* cameraBuffer{ nullptr };

		CPU_CAMERA cpu_camera{ Matrix() };
		DirectX::XMVECTOR view_determinant{0};

		void OnEnable() override;
		void OnDisable() override;
		void Awake() override;
		void LateUpdate() override;
		void OnPreRender() override;
		void OnDestroy() override;

	public:
		static Camera* mainCamera;
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
				GameState::GetCurrentScene()->ResourceManager.GetCBufferPreset(typeid(CPU_CAMERA).raw_name()))
			) {}


		void SetMainCamera() { mainCamera = this; }
		CPU_CAMERA GetCameraMatrix() { return CPU_CAMERA{ DirectX::XMMatrixTranspose(cpu_camera.view), DirectX::XMMatrixTranspose(cpu_camera.projection) }; }
	};
}
