#include "UEngine.h"
#include "Transform.h"

void UEngine::Transform::Awake()
{
	worldBuffer = DXRenderer::DXConstantBuffer::Instantiate
	(
		GetGameObject()->GetScene()->ResourceManager.GetNextCBufferID(),
		DXRenderer::Get(),
		DXRenderer::Get()->ResourceManager->GetCBufferPreset(typeid(CPU_WORLD).raw_name())
	);
	worldBuffer->AttachData(&world, sizeof(CPU_WORLD));
	GetGameObject()->GetScene()->ResourceManager.AddResource<DXRenderer::DXConstantBuffer>(worldBuffer->UID, worldBuffer);
}

void UEngine::Transform::Start()
{
	using namespace DirectX;
	RTP = XMMatrixMultiply
	(
		XMMatrixRotationRollPitchYaw(localRotation.value.x, localRotation.value.y, localRotation.value.z),
		XMMatrixTranslation(localPosition.value.x, localPosition.value.y, localPosition.value.z)
	);

	if (GetParent() != nullptr)
		RTP = XMMatrixMultiply(RTP, GetParent()->GetWorld());

	world.matrix = XMMatrixMultiply(XMMatrixScaling(scale.value.x, scale.value.y, scale.value.z), RTP);
	world.matrix = XMMatrixTranspose(world.matrix);
}

void UEngine::Transform::Update()
{
	auto renderComponent = GetComponent<RenderComponent>();
	if (renderComponent == nullptr) return;
	if (renderObject != renderComponent->GetRenderObject())
	{
		renderObject = renderComponent->GetRenderObject();
		if (renderObject == nullptr) return;
		renderComponent->AddConstantBuffer(worldBuffer);
	}
}

void UEngine::Transform::LateUpdate()
{
	//if (GetGameObject()->IsStatic) return;

	using namespace DirectX;
	RTP = XMMatrixMultiply
	(
		XMMatrixRotationRollPitchYaw(localRotation.value.x, localRotation.value.y, localRotation.value.z),
		XMMatrixTranslation(localPosition.value.x, localPosition.value.y, localPosition.value.z)
	);

	if (GetParent() != nullptr)
		RTP = XMMatrixMultiply(RTP, GetParent()->GetWorld());

	world.matrix = XMMatrixMultiply(XMMatrixScaling(scale.value.x, scale.value.y, scale.value.z), RTP);
	world.matrix = XMMatrixTranspose(world.matrix);
}

void UEngine::Transform::OnPreRender()
{
	worldBuffer->Update(DXRenderer::Get()->GetImmediateDeviceContext());
}

UEngine::Transform* UEngine::Transform::GetParent()
{
	auto goParent = GetGameObject()->GetParent();
	if (goParent == nullptr) return nullptr;
	return goParent->GetTransform();
}

UEngine::Transform* UEngine::Transform::GetChild(UINT index)
{
	auto goChild = GetGameObject()->GetChild(index);
	if (goChild == nullptr) return nullptr;
	return goChild->GetTransform();
}

std::vector<UEngine::Transform*> UEngine::Transform::GetChildren()
{
	auto goChildren = GetGameObject()->GetChildren();
	std::vector<Transform*> children;
	for (size_t i = 0; i < goChildren.size(); i++)
		children.push_back(goChildren[i]->GetTransform());
	return children;
}
