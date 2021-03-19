#include "UEngine.h"
#include "Camera.h"

void UEngine::Camera::SetMainCamera()
{ 
    if (GetGameObject()->GetScene()->MainCamera)
    {
        GetGameObject()->GetScene()->MainCamera->isThisMainCamera.value = false;
        GetGameObject()->GetScene()->MainCamera->gameView.IsMain = false;
    }
    this->isThisMainCamera.value = true;
    this->gameView.IsMain = true;
    GetGameObject()->GetScene()->MainCamera = this;
    GetGameObject()->GetScene()->MainView = &gameView;
}

void UEngine::Camera::OnEnable()
{
    if (GetGameObject()->GetScene()->MainCamera == nullptr) SetMainCamera();
    gameView.isRenderable = true;
}

void UEngine::Camera::OnDisable()
{
    if (!GameState::IsTerminate() && GetGameObject()->GetScene()->MainCamera == this)
        throw std::runtime_error("You cannot remove main camera!");
    gameView.isRenderable = false;
}

void UEngine::Camera::Awake()
{
    auto rendererDesc = DXRenderer::Get()->GetDescription();

    // View & Object Creation
    RECT windowSize;
    WinApplication::Get()->GetClientSize(&windowSize);
    view = DXRenderer::DXView::Instantiate
    (
        DXRenderer::Get(),
        windowSize.right - windowSize.left,
        windowSize.bottom - windowSize.top,
        rendererDesc.EnableDepthStencil,
        rendererDesc.MultisampleDesc
    );
    cameraBuffer->AttachData(&cpu_camera, sizeof(CPU_CAMERA));

    gameView.view = view;
    gameView.cameraBuffer = cameraBuffer;

    GetGameObject()->GetScene()->ResourceManager.AddResource<DXRenderer::DXConstantBuffer>(cameraBuffer->UID, cameraBuffer);
    GetGameObject()->GetScene()->ResourceManager.AddResource<DXRenderer::DXView>(view->UID, view);

    viewWidth.value = 40.0f;
    viewHeight.value = 40.0f;
    nearZ.value = -1.0f;
    farZ.value = 10.0f;
}

void UEngine::Camera::Update()
{
    GetGameObject()->GetScene()->cpu_view.emplace_back(gameView);
    GetGameObject()->GetScene()->cpu_view.emplace_back(gameView);
}

void UEngine::Camera::LateUpdate()
{
    DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(cameraRotation.value.x, cameraRotation.value.y, cameraRotation.value.z);
    DirectX::XMMATRIX position = DirectX::XMMatrixTranslation(cameraPosition.value.x, cameraPosition.value.y, cameraPosition.value.z);
    cpu_camera.view = DirectX::XMMatrixMultiply(rotation, position);
    cpu_camera.view.r[3] = DirectX::XMVector4Transform(cpu_camera.view.r[3], GetTransform()->GetRTP());
    view_determinant = DirectX::XMMatrixDeterminant(cpu_camera.view);

    cpu_camera.projection = DirectX::XMMatrixOrthographicLH(viewWidth.value, viewHeight.value, nearZ.value, farZ.value);

    cpu_camera = CPU_CAMERA
    {
        DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&view_determinant, cpu_camera.view)),
        DirectX::XMMatrixTranspose(cpu_camera.projection)
    };
}

void UEngine::Camera::OnPreRender()
{
    cameraBuffer->Update(DXRenderer::Get()->GetImmediateDeviceContext());
}

void UEngine::Camera::OnDestroy()
{
    for (size_t i = 0; i < GetGameObject()->GetScene()->cpu_view.size(); i++)
    {
        if (GetGameObject()->GetScene()->cpu_view[i].view->UID == gameView.view->UID)
            GetGameObject()->GetScene()->cpu_view.erase(GetGameObject()->GetScene()->cpu_view.begin() + i);
    }
    GetGameObject()->GetScene()->ResourceManager.RemoveResource<DXRenderer::DXView>(view->UID);
    GetGameObject()->GetScene()->ResourceManager.RemoveResource<DXRenderer::DXConstantBuffer>(cameraBuffer->UID);
}

void UEngine::Camera::DeSerialize(TiXmlNode* node)
{
    Serializer::DeSerialize(node);
    if (this->isThisMainCamera.value == true) SetMainCamera();
}

void UEngine::Camera::OnEditRender()
{
    Component::OnEditRender();
    if (isThisMainCamera.value && GetMainCamera() != this) SetMainCamera();
    else if (!isThisMainCamera.value && GetMainCamera() == this)
    {
        Console::WriteErrorLine("Cannot set false for the main camera");
        isThisMainCamera = true;
    }
}


