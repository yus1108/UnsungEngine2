#include "UEngine.h"
#include "Transform.h"

UEngine::Transform::Transform()
	: worldBuffer(DXRenderer::DXResourceManager::Get()->GetConstantBuffer("world"))
	, worldMat(DirectX::XMMatrixIdentity())
{
	worldBuffer->AttachData(&worldMat, sizeof(DirectX::XMMATRIX));
}

void UEngine::Transform::OnEnable()
{
	gameObject()->GetRenderObject()->AddConstantBuffer("world", worldBuffer);
}

void UEngine::Transform::Update()
{
	using namespace DirectX;

	worldMat = XMMatrixMultiply
	(
		XMMatrixScaling(scale.x, scale.y, scale.z), 
		XMMatrixRotationRollPitchYaw(localRotation.z, localRotation.x, localRotation.y)
	);
	worldMat = XMMatrixMultiply
	(
		worldMat,
		XMMatrixTranslation(localPosition.x, localPosition.y, localPosition.z)
	);
}
