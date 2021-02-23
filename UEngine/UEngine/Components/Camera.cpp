#include "UEngine.h"
#include "Camera.h"

UEngine::Camera::Camera()
    : cameraBuffer(DXRenderer::DXResourceManager::Get()->GetConstantBuffer("camera"))
{
    auto gameState = GameState::Get();
    auto rendererDesc = gameState->renderer->GetDescription();
    // View & Object Creation
    RECT windowSize;
    gameState->app->GetClientSize(&windowSize);
    view = DXRenderer::DXView::Instantiate
    (
        gameState->renderer,
        windowSize.right - windowSize.left,
        windowSize.bottom - windowSize.top,
        rendererDesc.EnableDepthStencil,
        rendererDesc.MultisampleDesc
    );
    gameState->cameras.emplace_back(this);

    viewWidth = 40;
    viewHeight = 30;
    nearZ = -1.0f;
    farZ = 100.0f;

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

    cameraBuffer->AttachData(&cpu_camera, sizeof(CPU_CAMERA));
}

UEngine::Camera::~Camera()
{
    auto cameras = GameState::Get()->cameras;
    for (auto camera = cameras.begin(); camera != cameras.end(); camera++)
    {
        if (*camera == this)
        {
            cameras.erase(camera);
            break;
        }
    }
    DXRenderer::DXView::Release(&view);
}
