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
    // resources mapping

    // thread safe
    {
        gameScene.OnRender();
        // gameobjects update
    }
    
    // thread join
    gameScene.OnPostRender();

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
        }

        std::cout << UEngine::Utility::UTime::Get()->FramePerSecond() << std::endl;
        std::cout << UEngine::Utility::UTime::Get()->DeltaTime() << std::endl;
    }
    
    threadPool.Join();
}
