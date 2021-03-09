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

        gameState->LoadObject([this](GameObject* background)
        {
            background->name = "background";
            background->AddComponent<RenderComponent>()->Load("rectangle", "sprite");
            background->AddComponent<Material>()->Load(L"./Assets/tiles and background_foreground/background.png", D3D11_TEXTURE_ADDRESS_WRAP, D3D11_FILTER_MIN_MAG_MIP_LINEAR);
            background->AddComponent<Background>();
            background->GetTransform()->scale = Vector2{
                static_cast<float>(screenSize.x),
                static_cast<float>(screenSize.y)
            };
            background->GetComponent<Material>()->uv = UV
            {
                0,
                0,
                1.0f / 5.0f,
                1
            };
            background->GetComponent<Background>()->speed = 0.5f;
        });

        for (size_t i = 0; i < 13; i++)
        {
            int index = i;
            gameState->LoadObject([this, index](GameObject* tile)
            {
                tile->name = "tile";
                tile->AddComponent<RenderComponent>()->Load("rectangle", "sprite");
                tile->AddComponent<Material>()->Load(L"./Assets/tiles and background_foreground/tileset.png");
                tile->AddComponent<Tile>();
                tile->AddComponent<Physics2D::RectCollider>()->SetCollider(100, 100);
                tile->GetTransform()->localPosition.x = -550 + index * 100;
                tile->GetTransform()->localPosition.y = -250;
                tile->GetTransform()->scale = Vector2{
                    100,
                    100
                };
                tile->GetComponent<Material>()->uv = UV
                {
                    7.0f / 12.0f,
                    0.5f,
                    8.0f / 12.0f,
                    1.0f - 1.0f / 3.0f
                };
            });
        }
        
        gameState->LoadObject([this](GameObject* player)
        {
            player->name = "player";
            player->AddComponent<RenderComponent>()->Load("rectangle", "sprite");
            player->AddComponent<Material>()->Load(L"./Assets/herochar sprites(new)/herochar_run_anim_strip_6.png");
            player->AddComponent<Player>();
            player->AddComponent<Physics2D::RectCollider>()->SetCollider(80, 80);
            player->GetTransform()->localPosition.x = -450;
            player->GetTransform()->localPosition.y = -50;
            player->GetTransform()->scale = Vector2{
                80,
                 80
            };
            player->GetComponent<Material>()->uv = UV
            {
                0,
                0,
                1.0f / 6.0f,
                1
            };
        });

        gameState->StartGame();
    }
}
 