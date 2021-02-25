#include "UEngine.h"
#include "Transform.h"

void UEngine::Transform::Awake()
{
	worldBuffer->AttachData(&world, sizeof(CPU_WORLD));
	GameState::Get()->constantBufferPool.Add(worldBuffer);
}

void UEngine::Transform::LateUpdate()
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

	auto renderComponent = GetComponent<RenderComponent>();
	if (renderComponent == nullptr) return;
	renderComponent->AddConstantBuffer(typeid(Transform).raw_name(), worldBuffer);
}