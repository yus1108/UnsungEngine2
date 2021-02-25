#include "UEngine.h"
#include "Camera.h"

UEngine::Camera* UEngine::Camera::mainCamera = nullptr;

void UEngine::Camera::OnEnable()
{
    if (mainCamera == nullptr) SetMainCamera();
}

void UEngine::Camera::OnDisable()
{
    if (!GameState::Get()->GetTerminate() && mainCamera == this) 
        throw std::runtime_error("You cannot remove main camera!");
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

    GameState::Get()->constantBufferPool.Add(cameraBuffer);
    GameState::Get()->gameScene.AddView(view);
}

void UEngine::Camera::Start()
{
    viewWidth = 40;
    viewHeight = 30;
    nearZ = -1.0f;
    farZ = 100.0f;
}

void UEngine::Camera::LateUpdate()
{
    DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(cameraRotation.z, cameraRotation.x, cameraRotation.y);
    DirectX::XMMATRIX position = DirectX::XMMatrixTranslation(cameraPosition.x, cameraPosition.y, cameraPosition.z);
    cpu_camera.view = DirectX::XMMatrixMultiply(rotation, position);
    //cpu_camera.view = DirectX::XMMatrixMultiply(cpu_camera.view, GetGameObject()->GetTransform()->GetRT());
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
    GameState::Get()->gameScene.RemoveView(view);
    GameState::Get()->constantBufferPool.Remove(cameraBuffer);
}


