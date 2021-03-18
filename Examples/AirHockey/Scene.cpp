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
            auto cameraObject = GameObject::Instantiate(currentScene, "camera");
            auto camera = cameraObject->AddComponent<Camera>();
            camera->viewWidth = 15;
            camera->viewHeight = 30;

            auto background = GameObject::Instantiate(currentScene, "background");
            background->GetTransform()->scale.value = Vector2(15, 30);
            background->AddComponent<RenderComponent>()->Load("rectangle", "image");
            background->AddComponent<Material>()->LoadImageMaterial(L"./Assets/soccer-field-4321814_640.jpg");
            currentScene->ResourceManager.ApplyChange();

            auto ball = GameObject::Instantiate(currentScene, "ball");
            ball->AddComponent<RenderComponent>()->Load("rectangle", "image");
            ball->AddComponent<Material>()->LoadImageMaterial(L"./Assets/football-157930_640.png");
            ball->AddComponent<Ball>();
            currentScene->ResourceManager.ApplyChange();

            auto p1 = GameObject::Instantiate(currentScene, "p1");
            p1->AddComponent<RenderComponent>()->Load("rectangle", "image");
            p1->AddComponent<Material>()->LoadImageMaterial(L"./Assets/football-157930_640.png");
            p1->GetComponent<Material>()->color = Color{ 1, 0, 0, 1 };
            p1->AddComponent<Player1>();
            p1->GetTransform()->localPosition.value = Vector2(0, 4);
            p1->GetTransform()->scale.value = Vector2(2, 2);
            currentScene->ResourceManager.ApplyChange();

            auto p2 = GameObject::Instantiate(currentScene, "p2");
            p2->AddComponent<RenderComponent>()->Load("rectangle", "image");
            p2->AddComponent<Material>()->LoadImageMaterial(L"./Assets/football-157930_640.png");
            p2->GetComponent<Material>()->color = Color{ 0, 0, 1, 1 };
            p2->AddComponent<Player2>();
            p2->GetTransform()->localPosition.value = Vector2(0, -4);
            p2->GetTransform()->scale.value = Vector2(2, 2);
            currentScene->ResourceManager.ApplyChange();

            auto goalPost1 = GameObject::Instantiate(currentScene, "goalPost1");
            goalPost1->AddComponent<RenderComponent>()->LoadRectangle();
            goalPost1->AddComponent<Material>()->color = Color{ 0, 0, 0, 0.5f };
            goalPost1->AddComponent<GoalPost>();
            goalPost1->GetTransform()->localPosition.value = Vector2(0, 14);
            goalPost1->GetTransform()->scale.value = Vector2(3.5f, 2);
            currentScene->ResourceManager.ApplyChange();

            auto goalPost2 = GameObject::Instantiate(currentScene, "goalPost2");
            goalPost2->AddComponent<RenderComponent>()->LoadRectangle();
            goalPost2->AddComponent<Material>()->color = Color{ 0, 0, 0, 0.5f };
            goalPost2->AddComponent<GoalPost>();
            goalPost2->GetTransform()->localPosition.value = Vector2(0, -14);
            goalPost2->GetTransform()->scale.value = Vector2(3.5f, 2);
            currentScene->ResourceManager.ApplyChange();
        }
        GameState::Init(currentScene);
    }
}

int Scene::Run()
{
    auto returnedValue = WinApplication::Get()->UpdateLoop([&]()
    {
        UEngine::Utility::UTime::Get()->Throttle(200);
        GameState::Update(nullptr, nullptr, nullptr);
    });

    return returnedValue;
}