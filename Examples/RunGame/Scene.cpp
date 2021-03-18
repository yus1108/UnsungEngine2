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
            camera->viewWidth = static_cast<float>(screenSize.x);
            camera->viewHeight = static_cast<float>(screenSize.y);

            auto background = GameObject::Instantiate(currentScene, "background");
            background->AddComponent<RenderComponent>()->Load("rectangle", "sprite");
            background->AddComponent<Material>()->LoadImageMaterial(L"./Assets/tiles and background_foreground/background.png", D3D11_TEXTURE_ADDRESS_WRAP, D3D11_FILTER_MIN_MAG_MIP_LINEAR);
            background->AddComponent<Background>();
            background->GetTransform()->scale.value = Vector2{
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
            currentScene->ResourceManager.ApplyChange();

            for (size_t i = 0; i < 13; i++)
            {
                int index = i;
                auto tile = GameObject::Instantiate(currentScene, "tile");
                tile->AddComponent<RenderComponent>()->Load("rectangle", "sprite");
                tile->AddComponent<Material>()->LoadImageMaterial(L"./Assets/tiles and background_foreground/tileset.png");
                auto tileScript = tile->AddComponent<Tile>();
                tile->AddComponent<Physics2D::RectCollider>()->SetCollider(100, 100);
                tileScript->fixedPosition.x = -550 + index * 100;
                tileScript->fixedPosition.y = -250;
                tile->GetTransform()->scale.value = Vector2{
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
                currentScene->ResourceManager.ApplyChange();
            }


            auto player = GameObject::Instantiate(currentScene, "player");
            player->AddComponent<RenderComponent>()->Load("rectangle", "sprite");
            player->AddComponent<Material>()->LoadImageMaterial(L"./Assets/herochar sprites(new)/herochar_run_anim_strip_6.png");
            player->AddComponent<Player>();
            player->AddComponent<Physics2D::RectCollider>()->SetCollider(50, 50);
            player->GetTransform()->localPosition.value.x = -450;
            player->GetTransform()->localPosition.value.y = -50;
            player->GetTransform()->scale.value = Vector2{
                50,
                 50
            };
            player->GetComponent<Material>()->uv = UV
            {
                0,
                0,
                1.0f / 6.0f,
                1
            };
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
 