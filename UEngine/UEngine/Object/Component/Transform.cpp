#include "UEngine.h"
#include "Transform.h"

UEngine::Transform::Transform()
	: worldBuffer(DXRenderer::DXConstantBuffer::Instantiate
	(
		DXRenderer::Get(),
		DXRenderer::DXResourceManager::Get()->GetConstantBuffer(typeid(CPU_WORLD).raw_name())
	))
{
	worldBuffer->AttachData(&world, sizeof(CPU_WORLD));
	GameState::Get()->constantBufferPool.Add(worldBuffer);
}

void UEngine::Transform::Update()
{
	using namespace DirectX;

	world.matrix = XMMatrixMultiply
	(
		XMMatrixScaling(scale.x, scale.y, scale.z), 
		XMMatrixRotationRollPitchYaw(localRotation.z, localRotation.x, localRotation.y)
	);
	world.matrix = XMMatrixMultiply
	(
		world.matrix,
		XMMatrixTranslation(localPosition.x, localPosition.y, localPosition.z)
	);

	world.matrix = XMMatrixTranspose(world.matrix);
}
