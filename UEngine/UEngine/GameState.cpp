#include "UEngine.h"
#include "GameState.h"

#include "..\WinApplication\WinConsole.h"
#include <iostream>

UEngine::GameState UEngine::GameState::instance;

void UEngine::GameState::Init(WinApplication* app, DXRenderer::DXRenderer* const renderer, size_t numThreads)
{
    this->app = app;
    this->renderer = renderer;
	threadPool.Init(numThreads);
}

void UEngine::GameState::Release()
{
    DXRenderer::DXView::Release(&currentView);
}

void UEngine::GameState::LoadScene(std::string file_name, std::function<void()> loadingScene)
{
    if (loadingScene) threadPool.AddSyncTask(loadingScene);

    auto rendererDesc = renderer->GetDescription();

    // View & Object Creation
    RECT windowSize;
    app->GetClientSize(&windowSize);
    currentView = DXRenderer::DXView::Instantiate
    (
        renderer,
        windowSize.right - windowSize.left,
        windowSize.bottom - windowSize.top,
        rendererDesc.EnableDepthStencil,
        rendererDesc.MultisampleDesc
    );
    DXRenderer::DXRenderObject* renderObj;
    {
        renderObj = DXRenderer::DXGeometryFigurePrefab::CreateCircle(36000);
        currentView->AddRenderObject(renderObj);
    }

    threadPool.Join();
}

void UEngine::GameState::Update()
{
    // constant buffers mapping
    currentView->UpdateConstantBuffers();
    renderer->UpdateConstantBuffers();

    // rendering in different thread
    threadPool.AddSyncTask([&]() {
        currentView->Begin();
        currentView->End(renderer->GetImmediateDeviceContext());

        renderer->Begin(DirectX::Colors::Transparent);
        renderer->GetImmediateDeviceContext()->PSSetShaderResources(0, 1, currentView->GetAddressOfViewResource());
        renderer->End();
    }); // render target view

    // update in main thread
    {
        UEngine::WinConsole::ResetCursorPos();
        std::cout << "\t\t" << std::endl;
        std::cout << "\t\t" << std::endl;
        UEngine::WinConsole::ResetCursorPos();

        std::cout << UEngine::Utility::UTime::Get()->FramePerSecond() << std::endl;
        std::cout << UEngine::Utility::UTime::Get()->DeltaTime() << std::endl;
    }

    threadPool.Join();
}
