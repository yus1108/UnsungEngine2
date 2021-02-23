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
    for (auto gameObject : gameObjects)
        GameObject::Release(&gameObject.second);
    gameObjects.clear();
    DXRenderer::DXView::Release(&currentView);
}

void UEngine::GameState::AddGameObject(std::string name, GameObject* gameObject)
{
    if (gameObjects[name]) throw std::runtime_error("GameObject with given name already exists");

    gameObjects[name] = gameObject;
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

    // adding gameobjects
    {
        auto gameObject = GameObject::Instantiate();
        auto renderObj = DXRenderer::DXGeometryFigurePrefab::CreateCircle(36000);
        gameObject->CopyRenderObject(renderObj);
        DXRenderer::DXRenderObject::Release(renderObj);
        AddGameObject("test", gameObject);
    }

    threadPool.Join();
}

void UEngine::GameState::Update()
{
    // constant buffers mapping
    for (auto gameObject : gameObjects)
        gameObject.second->GetRenderObject()->CBUpdateAll(currentView->GetDeviceContext());

    // rendering in different thread
    threadPool.AddSyncTask([&]() {
        currentView->Begin();
        for (auto gameObject : gameObjects)
        {
            gameObject.second->GetRenderObject()->Set(currentView->GetDeviceContext());
            gameObject.second->GetRenderObject()->Draw(currentView->GetDeviceContext());
        }
        currentView->End();
        currentView->Execute(renderer->GetImmediateDeviceContext());

        renderer->Begin(DirectX::Colors::Transparent);
        renderer->GetImmediateDeviceContext()->PSSetShaderResources(0, 1, currentView->GetAddressOfViewResource());
        renderer->End();
    }); // render target view

    // update in main thread
    {
        UEngine::WinConsole::ResetCursorPos();
        std::cout << "\t\t" << std::endl;
        std::cout << "\t\t" << std::endl;
        std::cout << "\t\t" << std::endl;
        UEngine::WinConsole::ResetCursorPos();

        if (UEngine::WinInput::Get()->GetKey(VK_LEFT))
        {
            std::cout << "key pressed" << std::endl;
            gameObjects["test"]->transform()->localPosition.x -= UEngine::Utility::UTime::Get()->DeltaTime();
        }

        for (auto gameObject : gameObjects)
        {
            gameObject.second->Update();
        }

        std::cout << UEngine::Utility::UTime::Get()->FramePerSecond() << std::endl;
        std::cout << UEngine::Utility::UTime::Get()->DeltaTime() << std::endl;
    }

    threadPool.Join();
}
