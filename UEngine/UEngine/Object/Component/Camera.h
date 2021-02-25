#pragma once
#include <iostream>

namespace UEngine
{
	class Camera : public IComponent
	{
	private:
		DXRenderer::DXConstantBuffer* cameraBuffer{ nullptr };

		CPU_CAMERA cpu_camera{ DirectX::XMMatrixIdentity() };
		DirectX::XMVECTOR view_determinant{0};

		void OnEnable() override;
		void OnDisable() override;
		virtual void Awake() override;
		virtual void Start() override;

	public:
		static Camera* mainCamera;
		DXRenderer::DXView* view{ nullptr };
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
				DXRenderer::DXResourceManager::Get()->GetConstantBuffer(typeid(CPU_CAMERA).raw_name()))
			){}


		void SetMainCamera() { GameState::Get()->gameScene.SetView(view); }
	};
}
