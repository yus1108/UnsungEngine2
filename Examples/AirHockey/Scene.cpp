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
}

void Scene::Load()
{
    // TODO: Place code here.
    {
        // basic load
        using namespace UEngine;
        GameScene* currentScene = new GameScene();
        currentScene->Init(true);

        {
            auto cameraObject = GameObject::Instantiate(currentScene, L"camera");
            auto camera = cameraObject->AddComponent<Camera>();
            camera->viewWidth = 15;
            camera->viewHeight = 30;

            auto background = GameObject::Instantiate(currentScene, L"background");
            background->GetTransform()->scale = Vector2(15, 30);
            background->AddComponent<RenderComponent>()->Load(L"rectangle", L"image");
            background->AddComponent<Material>()->Load(L"./Assets/soccer-field-4321814_640.jpg");

            auto ball = GameObject::Instantiate(currentScene, L"ball");
            ball->AddComponent<RenderComponent>()->Load(L"rectangle", L"image");
            ball->AddComponent<Material>()->Load(L"./Assets/football-157930_640.png");
            ball->AddComponent<Ball>();

            auto p1 = GameObject::Instantiate(currentScene, L"p1");
            p1->AddComponent<RenderComponent>()->Load(L"rectangle", L"image");
            p1->AddComponent<Material>()->Load(L"./Assets/football-157930_640.png");
            p1->GetComponent<Material>()->color = Color{ 1, 0, 0, 1 };
            p1->AddComponent<Player1>();
            p1->GetTransform()->localPosition = Vector2(0, 4);
            p1->GetTransform()->scale = Vector2(2, 2);

            auto p2 = GameObject::Instantiate(currentScene, L"p2");
            p2->AddComponent<RenderComponent>()->Load(L"rectangle", L"image");
            p2->AddComponent<Material>()->Load(L"./Assets/football-157930_640.png");
            p2->GetComponent<Material>()->color = Color{ 0, 0, 1, 1 };
            p2->AddComponent<Player2>();
            p2->GetTransform()->localPosition = Vector2(0, -4);
            p2->GetTransform()->scale = Vector2(2, 2);

            auto goalPost1 = GameObject::Instantiate(currentScene, L"goalPost1");
            goalPost1->AddComponent<RenderComponent>()->LoadRectangle();
            goalPost1->AddComponent<Material>()->color = Color{ 0, 0, 0, 0.5f };
            goalPost1->AddComponent<GoalPost>();
            goalPost1->GetTransform()->localPosition = Vector2(0, 14);
            goalPost1->GetTransform()->scale = Vector2(3.5f, 2);

            auto goalPost2 = GameObject::Instantiate(currentScene, L"goalPost2");
            goalPost2->AddComponent<RenderComponent>()->LoadRectangle();
            goalPost2->AddComponent<Material>()->color = Color{ 0, 0, 0, 0.5f };
            goalPost2->AddComponent<GoalPost>();
            goalPost2->GetTransform()->localPosition = Vector2(0, -14);
            goalPost2->GetTransform()->scale = Vector2(3.5f, 2);
        }
        GameState::Init(currentScene);
    }
}

int Scene::Run()
{
    auto returnedValue = WinApplication::Get()->UpdateLoop([&]()
    {
        UEngine::Utility::UTime::Get()->Throttle(200);
        GameState::Update(nullptr, nullptr);
    });

    return returnedValue;
}