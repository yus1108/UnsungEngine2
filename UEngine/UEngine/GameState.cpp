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

    // adding gameobjects
    {
        auto gameObject = GameObject::Instantiate();
        gameObject->AddComponent<RenderMesh>()->Load("circle");
        gameObject->AddComponent<Shader>()->Load("color");
        gameObject->AddComponent<Material>()->color = Color{ 1, 0, 0, 1 };
        gameObject->AddComponent<Camera>();
        AddGameObject("test", gameObject);
    }

    threadPool.Join();
}

void UEngine::GameState::Update()
{
    // constant buffers mapping
    for (size_t i = 0; i < cameras.size(); i++)
    {
        for (auto gameObject : gameObjects)
            gameObject.second->GetRenderObject()->CBUpdateAll(cameras[i]->view->GetDeviceContext());
    }

    for (size_t i = 0; i < cameras.size(); i++)
    {
        int index = i;
        threadPool.AddSyncTask([&]()
        {
            cameras[index]->view->Begin();
            for (auto gameObject : gameObjects)
            {
                gameObject.second->GetRenderObject()->Set(cameras[index]->view->GetDeviceContext());
                gameObject.second->GetRenderObject()->Draw(cameras[index]->view->GetDeviceContext());
            }
            cameras[index]->view->End();
        });
    }
    threadPool.Join();

    threadPool.AddSyncTask([&]()
    {
        for (size_t i = 0; i < cameras.size(); i++)
            cameras[i]->view->Execute(renderer->GetImmediateDeviceContext());

        renderer->Begin(DirectX::Colors::Transparent);
        renderer->GetImmediateDeviceContext()->PSSetShaderResources(0, 1, cameras[0]->view->GetAddressOfViewResource());
        renderer->End();
    });

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
