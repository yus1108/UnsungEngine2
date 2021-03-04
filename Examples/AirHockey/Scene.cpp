#include "stdafx.h"
#include "Scene.h"

Scene::Scene()
{
    // Window Application
    auto app = UEngine::WinApplication::Get();

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

        GameObject* background = GameObject::Instantiate();
        background->AddComponent<Transform>();
        background->GetTransform()->scale = Vector2(15, 30);
        background->AddComponent<RenderComponent>()->Load("rectangle", "basicTexture");
        background->AddComponent<Material>()->Load(L"./Assets/soccer-field-4321814_640.jpg");
        gameState->AddObject(background);

        GameObject* mainCamera = GameObject::Instantiate();
        mainCamera->AddComponent<Transform>();
        auto camera = mainCamera->AddComponent<Camera>();
        camera->viewWidth = 15;
        camera->viewHeight = 30;
        gameState->AddObject(mainCamera);

        GameObject* line = GameObject::Instantiate();
        line->name = "line";
        line->AddComponent<Transform>();
        auto lineTransform = line->GetTransform();
        lineTransform->scale = Vector2(1, 15);
        lineTransform->localRotation = Vector3(0, Utility::UMath::PI / 2.0f, 0);
        lineTransform->localPosition = Vector3(7.5f, 0, 0);
        line->AddComponent<RenderComponent>()->Load("line", "color");
        line->AddComponent<Material>()->color = Color{ 0, 0, 0, 1 };
        gameState->AddObject(line);

        GameObject* ball = GameObject::Instantiate();
        ball->name = "ball";
        ball->AddComponent<Transform>();
        ball->AddComponent<RenderComponent>()->Load("rectangle", "basicTexture");
        ball->AddComponent<Material>()->Load(L"./Assets/football-157930_640.png");
        ball->AddComponent<Ball>();
        gameState->AddObject(ball);

        GameObject* p1 = GameObject::Instantiate();
        p1->name = "p1";
        p1->AddComponent<Transform>();
        p1->AddComponent<RenderComponent>()->Load("rectangle", "basicTexture");
        p1->AddComponent<Material>()->Load(L"./Assets/football-157930_640.png");
        p1->GetComponent<Material>()->color = Color{ 1, 0, 0, 1 };
        p1->AddComponent<Player1>();
        p1->GetTransform()->localPosition = Vector2(0, 4);
        p1->GetTransform()->scale = Vector2(2, 2);
        gameState->AddObject(p1);

        GameObject* p2 = GameObject::Instantiate();
        p2->name = "p2";
        p2->AddComponent<Transform>();
        p2->AddComponent<RenderComponent>()->Load("rectangle", "basicTexture");
        p2->AddComponent<Material>()->Load(L"./Assets/football-157930_640.png");
        p2->GetComponent<Material>()->color = Color{ 0, 0, 1, 1 };
        p2->AddComponent<Player2>();
        p2->GetTransform()->localPosition = Vector2(0, -4);
        p2->GetTransform()->scale = Vector2(2, 2);
        gameState->AddObject(p2);

        GameObject* goalPost1 = GameObject::Instantiate();
        goalPost1->name = "goalPost1";
        goalPost1->AddComponent<Transform>();
        goalPost1->AddComponent<RenderComponent>()->LoadRectangle();
        goalPost1->AddComponent<Material>()->color = Color{ 0, 0, 0, 0.5f };
        goalPost1->AddComponent<GoalPost>();
        goalPost1->GetTransform()->localPosition = Vector2(0, 14);
        goalPost1->GetTransform()->scale = Vector2(3.5f, 2);
        gameState->AddObject(goalPost1);

        GameObject* goalPost2 = GameObject::Instantiate();
        goalPost2->name = "goalPost2";
        goalPost2->AddComponent<Transform>();
        goalPost2->AddComponent<RenderComponent>()->LoadRectangle();
        goalPost2->AddComponent<Material>()->color = Color{ 0, 0, 0, 0.5f };
        goalPost2->AddComponent<GoalPost>();
        goalPost2->GetTransform()->localPosition = Vector2(0, -14);
        goalPost2->GetTransform()->scale = Vector2(3.5f, 2);
        gameState->AddObject(goalPost2);
    }
}
