#include "stdafx.h"
#include "Scene.h"

Scene::Scene()
{
    // Window Application
    auto app = UEngine::WinApplication::Get();
    screenSize = app->GetClientPixelSize();

    // DXRenderer
    auto renderer = UEngine::DXRenderer::Get();
    UEngine::DXRenderer::RENDERER_DESC rendererDesc = UEngine::DXRenderer::CreateDefaultInitDesc();
    {
        rendererDesc.IsDebuggable = true;
        rendererDesc.EnableAntialisedLine = true;
        rendererDesc.EnableBlendState = true;
        rendererDesc.EnableDepthStencil = false;
        rendererDesc.EnableMultisampling = true;
        rendererDesc.MultisampleDesc = { 4, 0 };
        renderer->Init(app->GetHandler(), &rendererDesc);
    }

    auto gameState = UEngine::GameState::Get();
    gameState->Init(app, renderer);
}

void Scene::Load()
{
    auto gameState = UEngine::GameState::Get();
    // TODO: Place code here.
    {
        // basic load
        using namespace UEngine;
        GameObject* mainCamera = GameObject::Instantiate();
        mainCamera->AddComponent<Transform>();
        auto camera = mainCamera->AddComponent<Camera>();
        camera->viewWidth = screenSize.x;
        camera->viewHeight = screenSize.y;
        gameState->AddObject(mainCamera);


        GameObject* image = GameObject::Instantiate();
        image->name = "ball";
        image->AddComponent<Transform>();
        image->AddComponent<RenderComponent>()->Load("rectangle", "image");
        image->AddComponent<Material>()->Load(L"./Assets/island-5783440_1920.jpg");
        image->GetTransform()->scale = Vector2(screenSize.x, screenSize.y);
        gameState->AddObject(image);
       
    }
}
