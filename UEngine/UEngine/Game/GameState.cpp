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
    debugRenderer.Init(renderer->GetDevice(), renderer->GetImmediateDeviceContext());
}

void UEngine::GameState::Release()
{
    terminate = true;
    for (auto object : gameObjects)
        GameObject::Release(&object);
    gameObjects.clear();
}

void UEngine::GameState::Update()
{
    // cpu-gpu transfer
    gameScene.OnPreRender();
    constantBufferPool.OnPreRender();
    // resources mapping
    for (auto obj : gameObjects)
        obj->OnPreRender();

    // rendering thread
    gameScene.OnRender();
    // post render thread
    gameScene.OnPostRender();

    debugRenderer.Flush(gameScene.GetMainViewCBuffer());

    renderer->Begin();
    renderer->Draw(gameScene.GetMainView()->GetAddressOfViewResource());
    renderer->Draw(debugRenderer.GetViewResource());
    renderer->End();
    

    // update in main thread
    {
        UEngine::WinConsole::ResetCursorPos();
        std::cout << "\t\t" << std::endl;
        std::cout << "\t\t" << std::endl;
        std::cout << "\t\t" << std::endl;
        UEngine::WinConsole::ResetCursorPos();

        /*if (UEngine::WinInput::Get()->GetKey(VK_LEFT))
        {
            std::cout << "key pressed" << std::endl;
        }*/

        std::cout << UEngine::Utility::UTime::Get()->FramePerSecond() << std::endl;
        std::cout << UEngine::Utility::UTime::Get()->DeltaTime() << std::endl;
    }
    {
        // fixed timestamp update
        {
            for (auto obj : gameObjects)
                obj->FixedUpdate();
            for (auto obj : gameObjects)
                obj->PhysicsUpdate();
        }
        for (auto obj : gameObjects)
            obj->Update();
        for (auto obj : gameObjects)
            obj->LateUpdate();
        for (auto obj : gameObjects)
            obj->AnimationUpdate();
    }
    
    // thread join
    threadPool.Join();
    
    // post render update thread
    for (auto obj : gameObjects)
        obj->OnPostRender();


    // thread join
    threadPool.Join();
}
