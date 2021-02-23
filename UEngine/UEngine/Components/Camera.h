#pragma once

namespace UEngine
{
	class Camera : public IComponent
	{
	private:
		struct CPU_CAMERA
		{
			DirectX::XMMATRIX view;
			DirectX::XMMATRIX projection;
		};

	private:
		DXRenderer::DXConstantBuffer* cameraBuffer{ nullptr };

		CPU_CAMERA cpu_camera;
		DirectX::XMVECTOR view_determinant;

	public:
		DXRenderer::DXView* view{ nullptr };
		Vector3 cameraPosition{ 0, 0, 0 };
		Vector3 cameraRotation{ 0, 0, 0 };

		float viewWidth;
		float viewHeight;
		float nearZ;
		float farZ;

		Camera();
		~Camera();

		virtual void OnEnable() override { gameObject()->GetRenderObject()->AddConstantBuffer("camera", cameraBuffer); }
		virtual void OnDisable() override { gameObject()->GetRenderObject()->RemoveConstantBuffer("camera"); }
	};
}
