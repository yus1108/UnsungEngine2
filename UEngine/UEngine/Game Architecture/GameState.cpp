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

void UEngine::GameState::LoadObject(std::function<void(GameObject*)> function)
{
    auto gameObject = GameObject::Instantiate();
    std::pair<GameObject*, std::function<void(GameObject*)>> pair;
    pair.first = gameObject;
    pair.second = function;
    loads.push(pair);
}

void UEngine::GameState::StartGame()
{
    while (!loads.empty())
    {
        auto pair = loads.front();
        pair.second(pair.first);
        loads.pop();
    }
    for (auto obj : gameObjects)
        obj->Initialize();
}

void UEngine::GameState::Update()
{
    // cpu-gpu transfer
    gameScene.OnPreRender();
    ResourceManager.ConstantBufferPool.OnPreRender();

    // resources mapping
    for (auto obj : gameObjects)
        obj->OnPreRender();

    // rendering thread
    threadPool.AddSyncTask([&]()
    {
        gameScene.OnRender();
        gameScene.OnPostRender();

        debugRenderer.Flush(gameScene.GetMainViewCBuffer());

        renderer->Begin();
        renderer->Draw(gameScene.GetMainView()->GetAddressOfViewResource());
        renderer->Draw(debugRenderer.GetAddressOfViewResource());
        renderer->End();
    });

    // update in main thread
    {
        deltatime = UEngine::Utility::UTime::Get()->DeltaTimeF();
        fixedUpdateTimer += deltatime;

        UEngine::WinConsole::ResetCursorPos();
        std::cout << "\t\t" << std::endl;
        std::cout << "\t\t" << std::endl;
        std::cout << "\t\t" << std::endl;
        std::cout << "\t\t" << std::endl;
        UEngine::WinConsole::ResetCursorPos();

        std::cout << UEngine::Utility::UTime::Get()->FramePerSecond() << std::endl;
        std::cout << UEngine::Utility::UTime::Get()->DeltaTime() << std::endl;
    }
    {
        for (auto obj : gameObjects)
            obj->Initialize();

        // fixed timestamp update
        currentFixedTimestep = UEngine::Math::Clamp(deltatime, FixedTimestep, MaxFixedTimestep);
        while (fixedUpdateTimer > currentFixedTimestep)
        {
            currentFixedTimestep = UEngine::Math::Clamp(deltatime, FixedTimestep, MaxFixedTimestep);
            spatialPartition2d.Release();
            for (auto obj : gameObjects)
                obj->FixedUpdate();
            for (auto obj : gameObjects)
                obj->PhysicsUpdate();

            fixedUpdateTimer -= currentFixedTimestep;
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
}

void UEngine::GameState::EditorUpdate()
{
    for (auto obj : gameObjects)
        obj->Initialize();

    // fixed timestamp update
    currentFixedTimestep = UEngine::Math::Clamp(deltatime, FixedTimestep, MaxFixedTimestep);
    while (fixedUpdateTimer > currentFixedTimestep)
    {
        currentFixedTimestep = UEngine::Math::Clamp(deltatime, FixedTimestep, MaxFixedTimestep);
        spatialPartition2d.Release();
        for (auto obj : gameObjects)
            obj->FixedUpdate();
        for (auto obj : gameObjects)
            obj->PhysicsUpdate();

        fixedUpdateTimer -= currentFixedTimestep;
    }
    for (auto obj : gameObjects)
        obj->Update();
    for (auto obj : gameObjects)
        obj->LateUpdate();
    for (auto obj : gameObjects)
        obj->AnimationUpdate();

}

void UEngine::GameState::EditorRenderBegin()
{
    gameScene.OnRender();
    gameScene.OnPostRender();

    debugRenderer.Flush(gameScene.GetMainViewCBuffer());
    renderer->Begin();

    finalViewResources[0] = gameScene.GetMainView()->GetViewResource();
    finalViewResources[1] = debugRenderer.GetViewResource();
}

ID3D11ShaderResourceView** UEngine::GameState::EditorRenderEnd()
{
    renderer->End();
    for (auto obj : gameObjects)
        obj->OnPostRender();
    return finalViewResources;
}

void UEngine::GameState::EditorSyncData()
{
    // cpu-gpu transfer
    gameScene.OnPreRender();
    ResourceManager.ConstantBufferPool.OnPreRender();

    // resources mapping
    for (auto obj : gameObjects)
        obj->OnPreRender();
}
