#pragma once

namespace UEngine
{
	class Camera : public IComponent
	{
	private:
		DXRenderer::DXConstantBuffer* cameraBuffer{ nullptr };

		CPU_CAMERA cpu_camera;
		DirectX::XMVECTOR view_determinant;

	public:
		static Camera* mainCamera;
		DXRenderer::DXView* view{ nullptr };
		Vector3 cameraPosition{ 0, 0, 0 };
		Vector3 cameraRotation{ 0, 0, 0 };

		float viewWidth;
		float viewHeight;
		float nearZ;
		float farZ;

		Camera();
		~Camera();

		void SetMainCamera() { GameState::Get()->gameScene.SetView(view); }
	};
}
