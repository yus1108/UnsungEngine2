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
    for (auto object : gameObjects)
        GameObject::Release(&object);
    gameObjects.clear();
}

void UEngine::GameState::Update()
{
    // cpu-gpu transfer
    gameScene.OnPreRender();
    constantBufferPool.OnPreRender();
    for (auto obj : gameObjects)
        obj->OnPreRender();

    // resources mapping

    // rendering thread
    gameScene.OnRender();

    // update in main thread
    {
        /*UEngine::WinConsole::ResetCursorPos();
        std::cout << "\t\t" << std::endl;
        std::cout << "\t\t" << std::endl;
        std::cout << "\t\t" << std::endl;
        UEngine::WinConsole::ResetCursorPos();*/

        //if (UEngine::WinInput::Get()->GetKey(VK_LEFT))
        //{
        //    std::cout << "key pressed" << std::endl;
        //}

        //std::cout << UEngine::Utility::UTime::Get()->FramePerSecond() << std::endl;
        //std::cout << UEngine::Utility::UTime::Get()->DeltaTime() << std::endl;
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

    // onRender Update
    for (auto obj : gameObjects)
        obj->OnRender();

    // post render thread
    gameScene.OnPostRender();
    renderer->Begin(gameScene.GetMainView()->GetAddressOfViewResource());
    renderer->End();
    // post render update thread
    for (auto obj : gameObjects)
        obj->OnPostRender();


    // thread join
    threadPool.Join();
}
