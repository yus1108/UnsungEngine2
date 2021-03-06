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
        gameState->LoadObject([this](GameObject* cameraObject)
        {
            auto camera = cameraObject->AddComponent<Camera>();
            camera->viewWidth = static_cast<float>(screenSize.x);
            camera->viewHeight = static_cast<float>(screenSize.y);
        });
        
        gameState->LoadObject([this](GameObject* puzzleObj)
        {
            puzzleObj->name = "puzzle";
            auto puzzle = puzzleObj->AddComponent<Puzzle>();

            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    puzzle->pieces[i][j] = GameObject::Instantiate();
                    puzzle->pieces[i][j]->name = "pieces";
                    puzzle->pieces[i][j]->AddComponent<RenderComponent>()->Load("rectangle", "sprite");
                    puzzle->pieces[i][j]->AddComponent<Material>()->Load(L"./Assets/island-5783440_1920.jpg");
                    puzzle->pieces[i][j]->GetComponent<Material>()->uv = UV
                    {
                        1.0f / 3.0f * i,
                        1 - 1.0f / 3.0f * (j + 1),
                        1.0f / 3.0f * (i + 1),
                        1 - 1.0f / 3.0f * j,
                    };
                    puzzle->pieces[i][j]->GetTransform()->localPosition =
                        Vector2
                        (
                            static_cast<float>(screenSize.x) / 3.0f * (i - 1),
                            static_cast<float>(screenSize.y) / 3.0f * (j - 1)
                        );
                    puzzle->pieces[i][j]->GetTransform()->scale =
                        Vector2
                        (
                            static_cast<float>(screenSize.x) / 3.0f - 10,
                            static_cast<float>(screenSize.y) / 3.0f - 10
                        );
                }
            }
        });

        gameState->StartGame();
    }
}
 