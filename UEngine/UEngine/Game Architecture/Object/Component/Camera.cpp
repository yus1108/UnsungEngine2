#include "UEngine.h"
#include "Camera.h"

UEngine::Camera* UEngine::Camera::mainCamera = nullptr;

void UEngine::Camera::OnEnable()
{
    if (mainCamera == nullptr) SetMainCamera();
    if (gameView != nullptr) gameView->isRenderable = true;
}

void UEngine::Camera::OnDisable()
{
    if (!GameState::GetTerminate() && mainCamera == this) 
        throw std::runtime_error("You cannot remove main camera!");
    if (gameView != nullptr) gameView->isRenderable = false;
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

    gameView = new GameView;
    gameView->view = view;

    GetGameObject()->GetScene()->ResourceManager.AddResource<DXRenderer::DXConstantBuffer>(cameraBuffer->UID, cameraBuffer);
    GetGameObject()->GetScene()->ResourceManager.AddResource<DXRenderer::DXView>(view->UID, view);
    GetGameObject()->GetScene()->cpu_view.emplace_back(gameView);

    viewWidth = 40.0f;
    viewHeight = 40.0f;
    nearZ = -1.0f;
    farZ = 10.0f;
}

void UEngine::Camera::LateUpdate()
{
    DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(cameraRotation.z, cameraRotation.x, cameraRotation.y);
    DirectX::XMMATRIX position = DirectX::XMMatrixTranslation(cameraPosition.x, cameraPosition.y, cameraPosition.z);
    cpu_camera.view = DirectX::XMMatrixMultiply(rotation, position);
    cpu_camera.view = DirectX::XMMatrixMultiply(cpu_camera.view, GetTransform()->GetRTP());
    view_determinant = DirectX::XMMatrixDeterminant(cpu_camera.view);

    cpu_camera.projection = DirectX::XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ);

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
    GetGameObject()->GetScene()->ResourceManager.RemoveResource<DXRenderer::DXView>(view->UID);
    GetGameObject()->GetScene()->ResourceManager.RemoveResource<DXRenderer::DXConstantBuffer>(cameraBuffer->UID);
}


